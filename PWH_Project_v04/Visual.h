#pragma once

#include "olcPixelGameEngine.h"
#include "Plant.h"

class Visual : public olc::PixelGameEngine
{
private:
	olc::Sprite* ms_main_field{ nullptr }; // plant field background

	olc::Sprite* ms_plant     { nullptr }; // plant icon 
	olc::Decal*	 md_plant     { nullptr };

	olc::Sprite* ms_water_full{ nullptr }; // water amaunt icon
	olc::Decal*  md_water_full{ nullptr };
	olc::Sprite* ms_water_mid { nullptr };
	olc::Decal*  md_water_mid { nullptr };
	olc::Sprite* ms_water_low { nullptr };
	olc::Decal*  md_water_low { nullptr };
	
	olc::Sprite* ms_top_panel { nullptr }; // top panel with name
	olc::Decal*  md_top_panel { nullptr };
	olc::Sprite* ms_bot_panel { nullptr }; // just bottom panel
	olc::Decal*  md_bot_panel { nullptr }; // top and bottom panels are used for hiding objects

	int m_background_layer	   {}; // separate layer 
	int m_speed				   {}; // scroll mouse wheel
	std::vector<Plant> m_plants{}; // main data array
	olc::vi2d m_mouse_pos      {}; 
public:
	Visual();

	bool OnUserCreate() override;
	bool OnUserUpdate(float elapsed_time) override;

	void drawStringOutline(const olc::vi2d& pos, const std::string& str, const olc::Pixel& color, int size); // must have function if you drawing text
	void drawButton(const olc::vi2d& pos, const olc::vi2d& size, const std::string& str, bool active, int thickness = 4);
	void drawPlants();
};

