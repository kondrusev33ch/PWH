#include "Visual.h"
#include "Helpers.h"
#include "Logic.h"

// Buttons
const olc::vi2d g_btn_size       { 140,  80  };
const olc::vi2d g_btn_add_pos    { 1120, 60  };
const olc::vi2d g_btn_del_pos    { 1120, 160 };
const olc::vi2d g_btn_water_pos  { 1120, 260 };
const olc::vi2d g_btn_exit_pos   { 1120, 620 };
							      
// Main 				      
const olc::vi2d g_field_pos	     { 20,   60  };
const olc::vi2d g_field_size     { 1080, 640 };
const olc::vi2d g_bot_panel_pos  { 0,    700 };

// Colors
const olc::Pixel g_outline_color { 116, 70,  18,  255 };
const olc::Pixel g_main_color    { 181, 110, 28,  255 };
const olc::Pixel g_button_color  { 255, 234, 192, 255 };
const olc::Pixel g_cell_color    { 86,  34,  4,   170 };
const olc::Pixel g_back_color    { 204, 255, 229, 255 };

// Decor
constexpr auto g_img_start_pos_x { 40 };
constexpr auto g_img_start_pos_y { 80 };
constexpr auto g_img_scale_w     { 3.0f };
constexpr auto g_img_scale_h     { 2.76f };
constexpr auto g_indent_length   { 20 };
constexpr auto g_object_length   { 510 };
constexpr auto g_max_visible_obj { 8 };
constexpr auto g_name_indent     { 30 };
constexpr auto g_magic           { 430 };
int g_img_plant_height   {};
int g_img_plant_width    {};
int g_plant_object_height{};

Visual::Visual()
{
	sAppName = "Watering helper";
}

bool Visual::OnUserCreate()
{
	// Sprites
	ms_main_field = new olc::Sprite("resources\\wood.png");
	ms_plant      = new olc::Sprite("resources\\plant.png");
	ms_water_full = new olc::Sprite("resources\\water_full.png");
	ms_water_mid  = new olc::Sprite("resources\\water_mid.png");
	ms_water_low  = new olc::Sprite("resources\\water_low.png");
	ms_top_panel  = new olc::Sprite("resources\\top_panel.png");
	ms_bot_panel  = new olc::Sprite("resources\\bot_panel.png");

	// Decals
	md_plant      = new olc::Decal(ms_plant);
	md_water_full = new olc::Decal(ms_water_full);
	md_water_mid  = new olc::Decal(ms_water_mid);
	md_water_low  = new olc::Decal(ms_water_low);
	md_top_panel  = new olc::Decal(ms_top_panel);
	md_bot_panel  = new olc::Decal(ms_bot_panel);

	// Globals
	g_img_plant_height    = static_cast<int>(ms_plant->height * g_img_scale_h);
	g_img_plant_width     = static_cast<int>(ms_plant->width * g_img_scale_w);
	g_plant_object_height = g_indent_length + g_img_plant_height;

	// Create background layer
	m_background_layer = CreateLayer();
	SetDrawTarget(m_background_layer);

	Clear(g_back_color); // set background color
	DrawSprite(g_field_pos, ms_main_field); // draw wooden field

	EnableLayer(m_background_layer, true);
	SetDrawTarget(nullptr);

	// Get saved data
	if (DataMngr::isFileExist(g_save_file_name))
		if (!DataMngr::isFileEmpty(g_save_file_name))
			if (DataMngr::readFromFile(m_plants, g_save_file_name))
				for (auto& plant : m_plants)
					plant.updateInfo();

	// Sort plants array based on how many days left to the next watering day
	Logic::sortPlantArray(m_plants);

	return true;
}

bool Visual::OnUserUpdate(float elapsed_time)
{
	Clear(olc::BLANK);

	// Information updates in case programm will work nonstop
	static int update{ 3000 };
	if (update == 0)
	{
		for (auto& plant : m_plants)
			plant.updateInfo();

		update = 3000;
	}
	update--;

	// Main draw function
	drawPlants();

	// Get mouse position to collision detection
	m_mouse_pos = { GetMouseX(), GetMouseY() };

	// Add button logic
	if (Logic::isColliding(g_btn_add_pos, g_btn_size, m_mouse_pos)) // if the mouse is on the button
	{
		drawButton(g_btn_add_pos, g_btn_size, "Add", true);
		if (GetMouse(0).bPressed) // if the mouse was pressed
			Logic::getInput(m_plants, Logic::getPlant); // get input from the user to add a plant
	}
	else
		drawButton(g_btn_add_pos, g_btn_size, "Add", false);

	if (m_plants.size() > 0) // if there are no plants, then no need to delete or water
	{
		// Delete button logic
		if (Logic::isColliding(g_btn_del_pos, g_btn_size, m_mouse_pos))
		{
			drawButton(g_btn_del_pos, g_btn_size, "Del", true);
			if (GetMouse(0).bPressed)
				Logic::getInput(m_plants, Logic::deletePlant); // get input from the user to delete a plant
		}
		else
			drawButton(g_btn_del_pos, g_btn_size, "Del", false);

		// Water button logic
		if (Logic::isColliding(g_btn_water_pos, g_btn_size, m_mouse_pos))
		{
			drawButton(g_btn_water_pos, g_btn_size, "Water", true);
			if (GetMouse(0).bPressed)
				if (Logic::confirmMBox("Watering", "Simple comfirm, protects you from misclick"))
				{
					bool is_needed{ false }; // nothing will happens if no need

					for (auto& plant : m_plants)
						if (plant.getDaysLeft() < 1)
						{
							plant.wateringWasDone();
							is_needed = true;
						}

					if (is_needed)
					{
						// Update the save file
						DataMngr::deleteFile(g_save_file_name);
						for (auto& plant : m_plants)
							DataMngr::writeToFile(plant, g_save_file_name);

						Logic::sortPlantArray(m_plants);
					}
				}
		}
		else
			drawButton(g_btn_water_pos, g_btn_size, "Water", false);
	}

	// Exit button logic
	if (Logic::isColliding(g_btn_exit_pos, g_btn_size, m_mouse_pos))
	{
		drawButton(g_btn_exit_pos, g_btn_size, "Exit", true);
		if (GetMouse(0).bPressed)
		{
			if (!m_plants.empty())
			{
				// Update save file in case something bad happens to save file during process runs
				DataMngr::deleteFile(g_save_file_name);

				for (auto& plant : m_plants)
					DataMngr::writeToFile(plant, g_save_file_name);
			}

			return false;
		}
	}
	else
		drawButton(g_btn_exit_pos, g_btn_size, "Exit", false);

	// Main field logic
	if (m_plants.size() > g_max_visible_obj) // if objects number is lower than 9, then no need to scroll be enabled
		if (Logic::isColliding(g_field_pos, g_field_size, m_mouse_pos))
		{
			if (GetMouseWheel() < 0)
			{
				int current_array_hight{
					(static_cast<int>(m_plants.size()) / 2 + (static_cast<int>(m_plants.size()) % 2)) * g_plant_object_height };
				int accesable_array_hight{ (g_max_visible_obj / 2) * g_plant_object_height };
				if ((accesable_array_hight - current_array_hight) < m_speed) // set limits
					m_speed -= g_plant_object_height / 5;
			}

			if (GetMouseWheel() > 0)
				if (m_speed < 0) // set limits
					m_speed += g_plant_object_height / 5;
		}

	// Draw panels to hide objects you don't need to see
	DrawDecal(olc::vi2d{}, md_top_panel);
	DrawDecal(g_bot_panel_pos, md_bot_panel);

	return true;
}

void Visual::drawStringOutline(const olc::vi2d& pos, const std::string& str, const olc::Pixel& color, int size)
{
	DrawString(olc::vi2d{ pos.x + 2, pos.y + 2 }, str, olc::BLACK, size);
	DrawString(pos, str, color, size);
}

void Visual::drawButton(const olc::vi2d& pos, const olc::vi2d& size, const std::string& str, bool active, int thickness)
{
	olc::Pixel color{ active ? olc::WHITE : g_button_color };
	FillRect(pos, size, g_outline_color);
	FillRect(olc::vi2d{ pos.x + thickness, pos.y + thickness },
		olc::vi2d{ size.x - thickness * 2, size.y - thickness * 2 }, color);

	// I know that 3 font is 20*23 or close, and I need them only here, so let them be magic, plz
	olc::vi2d text_center{ pos.x + (size.x - (static_cast<int>(str.length()) * 23)) / 2,
						   pos.y + (size.y - 20) / 2 };
	drawStringOutline(text_center, str, g_main_color, 3);
}

void Visual::drawPlants()
{
	olc::vi2d img_pos{};
	olc::vi2d txt_pos{};

	for (int i{ 0 }; i < m_plants.size(); i++)
	{
		// Find base position
		if (!(i % 2)) // we have 2 object in 1 line, so every third go to the next line and become first ... 
		{
			img_pos = {
				g_img_start_pos_x,
				g_img_start_pos_y + (g_img_plant_height + g_indent_length) * (i / 2) + m_speed }; // m_speed for scrolling

			txt_pos = {
				img_pos.x + g_img_plant_width + g_name_indent,
				img_pos.y + g_indent_length };
		}
		else
		{
			img_pos = {
				g_img_start_pos_x + g_object_length + g_indent_length,
				g_img_start_pos_y + (g_img_plant_height + g_indent_length) * (i / 2) + m_speed };

			txt_pos = {
				img_pos.x + g_img_plant_width + g_name_indent,
				img_pos.y + g_indent_length };
		}

		// Object background and icon
		FillRect(img_pos, { g_object_length, g_img_plant_height }, g_cell_color); // object dark background
		DrawRect(img_pos, { g_object_length, g_img_plant_height }, olc::WHITE);   // white frame
		DrawDecal(img_pos, md_plant, { g_img_scale_w, g_img_scale_h });           // and plant image

		// Water amount indicator
		switch (m_plants[i].getWaterAmount())
		{
		case 1: // low amount
			DrawDecal(olc::vi2d{ img_pos.x + g_magic, img_pos.y + (g_img_plant_height - 
				static_cast<int>(ms_water_low->height * g_img_scale_h)) / 2 }, md_water_low, { g_img_scale_w, g_img_scale_h });
			break;

		case 2: // medium amount
			DrawDecal(olc::vi2d{ img_pos.x + g_magic, img_pos.y + (g_img_plant_height -
				static_cast<int>(ms_water_mid->height * g_img_scale_h)) / 2 }, md_water_mid, { g_img_scale_w, g_img_scale_h });
			break;

		case 3: // hight amount
			DrawDecal(olc::vi2d{ img_pos.x + g_magic, img_pos.y + (g_img_plant_height -
				static_cast<int>(ms_water_full->height * g_img_scale_h)) / 2 }, md_water_full, { g_img_scale_w, g_img_scale_h });
			break;
		}

		// Plant information 
		drawStringOutline(txt_pos, m_plants[i].getName(), olc::WHITE, 2);

		int indent_after_name   { 33 };
		int indent_after_id     { 60 };
		int indent_between_lines{ 25 };

		drawStringOutline(olc::vi2d{ txt_pos.x, txt_pos.y + indent_after_name }, "ID: ", olc::DARK_GREY, 2);
		drawStringOutline(olc::vi2d{ txt_pos.x + indent_after_id, txt_pos.y + indent_after_name }, 
			std::to_string(m_plants[i].getID()), olc::WHITE, 2);

		drawStringOutline(olc::vi2d{ txt_pos.x, txt_pos.y + indent_after_name + indent_between_lines }, 
			"Days left: ", olc::DARK_GREY, 2);

		if (m_plants[i].getDaysLeft() < 1)
			drawStringOutline(olc::vi2d{ txt_pos.x, txt_pos.y + indent_after_name + indent_between_lines * 2 }, 
				"Today morning!", olc::RED, 2);
		else
			drawStringOutline(olc::vi2d{ txt_pos.x, txt_pos.y + indent_after_name + indent_between_lines * 2 }, 
				std::to_string(m_plants[i].getDaysLeft()), olc::WHITE, 2);
	}
}
