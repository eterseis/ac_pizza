#include "settings.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <fstream>
#include <iostream>

bool Settings::SaveConfig(char* file_name)
{
	::CreateDirectory("C:/mozart/", nullptr);
	char path[32];
	strcpy_s(path, "C:/mozart/");
	strcat_s(path, file_name);
	strcat_s(path, "\0");

	std::ofstream f(path, std::ios::binary);

	if (!f.is_open()) return 0;

	/* increase the address of this object by 1 byte. So that it skips "m_ShowMenu". */
	f.write((reinterpret_cast<char*>(&(*this))) + 1, sizeof(*this) - 1);
	f.close();

	return 1;
}

bool Settings::LoadConfig(char* file_name)
{
	char path[32];
	strcpy_s(path, "C:/mozart/");
	strcat_s(path, file_name);
	strcat_s(path, "\0");

	std::ifstream f(path, std::ios::binary);

	if (!f.is_open()) return 0;

	/* increase the address of this object by 1 byte. So that it skips "m_ShowMenu". */
	f.read((reinterpret_cast<char*>(&(*this))) + 1, sizeof(*this) - 1);
	f.close();

	return 1;
}