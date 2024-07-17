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
	std::ofstream file(fileName);

	file << content;

	file.close();
}
