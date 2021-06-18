#include "Helpers.h"

#include <fstream> // std::fstream
#include <sys/stat.h> // stat
#include <iostream>

HWND g_HWND{};

BOOL Mem::EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		g_HWND = hwnd;
		return FALSE;
	}
	return TRUE;
}

HWND Mem::GetHWND(int32_t process_id)
{
	EnumWindows(EnumWindowsProcMy, process_id);
	return g_HWND;
}

bool DataMngr::isFileExist(std::string_view file_name) // fastest way 
{
	struct stat buffer {};
	return (stat(file_name.data(), &buffer) == 0);
}

bool DataMngr::deleteFile(std::string_view file_name)
{
	if (DataMngr::isFileExist(file_name))
	{
		std::remove(file_name.data());
		return true;
	}

	return false;
}

bool DataMngr::isFileEmpty(const std::string& file_name) 
{
	std::ifstream inf{ file_name, std::ios::ate }; // move pointer to the end of the file
	return (inf.tellg() == 0); // if pointer is null, this is empty file 
} // inf goes out of scope here, no need to .close()

bool DataMngr::writeToFile(const Plant& data, const std::string& file_name)
{
	std::ofstream outf{ file_name, std::ios::app };
	if (outf.is_open())
	{
		outf << data << std::endl;

		if (outf.good())
			return true;
		else
			return false;
	}
	else
		return false;
}

bool DataMngr::readFromFile(std::vector<Plant>& array, const std::string& file_name)
{
	std::ifstream inf{ file_name };
	if (inf.is_open())
	{
		array.clear();
		array.resize(0);

		Plant plant{};
		while (inf >> plant)
			array.push_back(std::move(plant));

		return true;
	}

	return false;
}

void Stream::clearStream()
{
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

const std::string Stream::getString(int max_length)
{
	std::string result{};
	std::getline(std::cin, result);

	if (result.size() > max_length)
		result.resize(max_length);

	result.erase(remove_if(result.begin(), result.end(), std::isspace), result.end()); // spaces is not allowed because of save system

	return result;
}