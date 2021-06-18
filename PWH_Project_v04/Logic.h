#pragma once

#include "olcPixelGameEngine.h"
#include "Plant.h"

#include <vector>
#include <functional>

constexpr auto g_save_file_name{ "PWH_save.dat" };

namespace Logic
{
	void sortPlantArray(std::vector<Plant>& plants);
	bool isColliding(const olc::vi2d& obj_pos, const olc::vi2d& obj_size, const olc::vi2d& mouse_pos);
	void getInput(std::vector<Plant>& plants, std::function<void(std::vector<Plant>& plants)> func);
	void getPlant(std::vector<Plant>& plants);
	void deletePlant(std::vector<Plant>& plants);
	bool confirmMBox(const std::string& title, const std::string& txt);
}

