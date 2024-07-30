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

#pragma once

#include "exumeta.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <vector>

class Log
{
private:
	std::filesystem::path logPath;
	int logLevel; // 0 = off | 1 = Error | 2 = Warning | 3 = Info

	void CheckSize()
	{
		auto size = std::filesystem::file_size(logPath);
		if (size/1000 > 100) // delete file if bigger than 100 kb
		{
			std::filesystem::remove(logPath);
		}
	}

	bool CheckEmpty(const std::filesystem::path& filePath)
	{
		std::ifstream file(filePath, std::ios::ate);
		return file.tellg() == 0; // return 0 if file is empty
	}

	void LogInit()
	{
		CheckSize();
		std::ofstream file(logPath, std::ios::app);
		if (!CheckEmpty(logPath)) { file << '\n'; }
		file.close();
		Out(std::format("Extra Utilities started successfully! Version: {}", Exu::version));
		Out(std::format("Logging level is: {}", logLevel));
	}

public:
	Log()
	{
		logPath = ".\\Extra Utilities\\exulog.txt";
		logLevel = 3;
		LogInit();
	}

	Log(const std::filesystem::path& path, int level)
	{
		logPath = path;
		logLevel = level;
		LogInit();
	}

	void Out(const std::string& content, const int level = 3)
	{
		if (logLevel < level)
			return;

		auto now = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
		std::tm localTime = *std::localtime(&currentTime);

		std::ofstream file(logPath, std::ios::app);
		file << std::put_time(&localTime, "%H:%M:%S") << ": " << content << '\n';
		file.close();
	}

	int GetLogLevel()
	{
		return logLevel;
	}
	
	void SetLogLevel(int level)
	{
		logLevel = level;
	}

	std::filesystem::path GetLogPath()
	{
		return logPath;
	}

	void SetLogPath(const std::filesystem::path& path)
	{
		logPath = path;
	}
};

extern Log* SystemLog;