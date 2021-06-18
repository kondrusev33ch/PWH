#pragma once

#include "Plant.h"

#include <vector>
#include <string>
#include <string_view>
#include <cstdint>

namespace Mem
{
	BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam);
	HWND GetHWND(int32_t process_id);
}

namespace DataMngr
{
	bool isFileExist(std::string_view file_name);
	bool deleteFile(std::string_view file_name);
	bool isFileEmpty(const std::string& file_name);
	bool writeToFile(const Plant& data, const std::string& file_name);
	bool readFromFile(std::vector<Plant>& array, const std::string& file_name);
}

namespace Stream
{
	void clearStream();
	const std::string getString(int max_length);
}