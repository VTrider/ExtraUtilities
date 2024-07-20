#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>



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

	void LogInit()
	{
		CheckSize();
		std::ofstream file(logPath, std::ios::app);
		file << '\n';
		file << "Extra Utilities started successfully!" << " Version: " << "0.5.3" << '\n';
		file << "Logging level is: " << logLevel << '\n';
		file.close();
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