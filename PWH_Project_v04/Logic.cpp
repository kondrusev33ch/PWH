#include "Logic.h"
#include "Helpers.h"

#include <algorithm>

constexpr auto g_max_name_length{ 20 };

void Logic::sortPlantArray(std::vector<Plant>& plants)
{
	if (plants.size() > 1)
		std::sort(plants.begin(), plants.end(), [](Plant& p1, Plant& p2) { return p1.getDaysLeft() < p2.getDaysLeft(); });
}

bool Logic::isColliding(const olc::vi2d& obj_pos, const olc::vi2d& obj_size, const olc::vi2d& mouse_pos)
{
	return mouse_pos.x >= obj_pos.x && mouse_pos.y >= obj_pos.y && mouse_pos.x
		<= (obj_pos.x + obj_size.x) && mouse_pos.y <= (obj_pos.y + obj_size.y);
}

void Logic::getInput(std::vector<Plant>& plants, std::function<void(std::vector<Plant>& plants)> func)
{
	HWND main_wnd{ Mem::GetHWND(GetCurrentProcessId()) };

	if (!AllocConsole())
		MessageBox(0, L"The console window was not created", 0, MB_ICONEXCLAMATION);

	FILE* out_f;
	FILE* in_f;
	freopen_s(&out_f, "CONOUT$", "w", stdout);
	freopen_s(&in_f, "CONIN$", "r", stdin);

	HWND h_console{ GetConsoleWindow() };

	RECT main_rect{};
	GetWindowRect(main_wnd, &main_rect);
	MoveWindow(h_console, main_rect.left + 20, main_rect.top + 90, 1095, 650, TRUE);

	func(plants);
	sortPlantArray(plants);

	if (out_f)
		fclose(out_f);

	if (in_f)
		fclose(in_f);

	if (!FreeConsole())
		MessageBox(0, L"Failed to free the console!", 0, MB_ICONEXCLAMATION);
}

void Logic::getPlant(std::vector<Plant>& plants)
{
	std::string name{};
	char save{};
	int in_warm{};
	int in_cold{};
	int water_amount{};

	do
	{
		if (save)
			std::cout << "You pressed wrong button! You better follow what I say, or you will be stuck\n \n";

		std::cout << "Plant #" << plants.size() + 1 << '\n';
		std::cout << "Enter name (do not use space bar): ";
		name = Stream::getString(g_max_name_length);

		std::cout << "WARM -- from Apr to Sep | COLD -- from Oct to Mar\n";

		std::cout << "In WARM months, the plant should be watered every (days): ";
		std::cin >> in_warm;
		Stream::clearStream(); // in case bad input has occure

		std::cout << "In COLD months, the plant should be watered every (days): ";
		std::cin >> in_cold;
		Stream::clearStream();

		do
		{
			std::cout << "Choose water amount [1/2/3] where 1 = low, 2 = medium, 3 = high: ";
			std::cin >> water_amount;
			Stream::clearStream();
		} while (water_amount != 1 && water_amount != 2 && water_amount != 3);

		std::cout << "Save? [y/n]: ";
		std::cin >> save;
		Stream::clearStream();

		if (save == 'n')
			return;

	} while (save != 'y');

	int id{};
	if (plants.empty())
		id = 1;
	else
	{ // Because we sort array, we need to find one with largest ID to create new plant with correct ID
		auto lambda{ [](const Plant& p1, const Plant& p2) {return p1.getID() < p2.getID(); } };
		id = std::max_element(plants.begin(), plants.end(), lambda)->getID() + 1;
	}

	plants.push_back({ name, id, in_warm, in_cold, water_amount });
	DataMngr::writeToFile(plants.back(), g_save_file_name);
}

void Logic::deletePlant(std::vector<Plant>& plants)
{
	int id{};
	char confirm{};

	std::sort(plants.begin(), plants.end(), [](Plant& p1, Plant& p2) {return p1.getName() < p2.getName(); });

	do
	{
		if (confirm)
			std::cout << "You pressed wrong button! You better follow what I say, or you will be stuck\n \n";

		for (const auto& plant : plants)
		{
			std::cout << std::setw(g_max_name_length) << std::left << plant.getName() << " ID: ";
			std::cout << plant.getID() << '\n';
		}

		std::cout << "\nEnter plant ID you want to delete: ";
		std::cin >> id;
		Stream::clearStream();

		std::cout << "Are you shure? [y/n]: ";
		std::cin >> confirm;
		Stream::clearStream();

		if (confirm == 'n')
			return;

	} while (confirm != 'y');

	for (int i{ 0 }; i < plants.size(); i++)
		if (plants[i].getID() == id)
		{
			plants.erase(plants.begin() + i);
			DataMngr::deleteFile(g_save_file_name);

			for (const auto& plant : plants)
				DataMngr::writeToFile(plant, g_save_file_name);

			break;
		}
}

bool Logic::confirmMBox(const std::string& title, const std::string& txt)
{
	std::wstring w_title = std::wstring(title.begin(), title.end());
	std::wstring w_txt = std::wstring(txt.begin(), txt.end());

	const int result = MessageBox(0, w_txt.c_str(), w_title.c_str(), MB_YESNOCANCEL | MB_ICONQUESTION);

	if (result == IDYES)
		return true;

	return false;
}
