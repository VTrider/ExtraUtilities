/* Copyright (C) 2023-2024 VTrider
 *
 * This file is part of Extra Utilities.
 *
 * Extra Utilities is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// File Handling goes here

#include "filesystem.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "Log.h"

extern Log* SystemLog;

void CreateEXUDirectory()
{
	if (!std::filesystem::is_directory("Extra Utilities") || !std::filesystem::exists("Extra Utilities"))
	{
		std::filesystem::create_directory("Extra Utilities");
	}
}

std::string GetWorkingDirectory()
{
	return std::filesystem::current_path().string();
}

void MakeDirectory(const char* directory)
{
	std::filesystem::create_directories(directory);
}

const char* FileRead(const char* fileName)
{
	std::ifstream file(fileName);

	std::string content;
	std::string line;

	while (std::getline(file, line))
	{
		content += line + "\n";
	}

	file.close();

	return content.c_str();
}

void FileWrite(const char* fileName, const char* content)
{
	std::filesystem::path path = fileName;
	if (!std::filesystem::exists(path.parent_path()))
	{
		std::filesystem::create_directories(path.parent_path());
	}

	std::ofstream file(fileName);

	file << content;

	file.close();
}
