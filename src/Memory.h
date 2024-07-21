#pragma once

#include "bzr.h"
#include "Log.h"
#include "Offsets.h"

#include <Windows.h>

#include <format>
#include <unordered_map>
#include <variant>
#include <vector>

// todo: restore values

extern Log* SystemLog;

class Memory
{
private:
	static inline bool useWinApi = false;

	static inline HWND hWnd{};
	static inline DWORD pid{};
	static inline HANDLE pHandle{};
	static inline DWORD moduleBase{}; 

	static inline DWORD dummyOldProtection{}; // required arg for VirtualProtect

	static inline int checkInterval{};
	static inline auto nextCheck = std::chrono::steady_clock::now() + std::chrono::seconds(checkInterval);

public:
	static void Init()
	{
		hWnd = FindWindowA(0, ("Battlezone 98 Redux (2.2.301)"));
		GetWindowThreadProcessId(hWnd, &pid);
		pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		moduleBase = (DWORD)GetModuleHandle("Battlezone98Redux.exe"); // need to get the base address of the game in order to deal with multilevel pointers
	}

	static void SetAccessMode(int mode)
	{
		switch (mode)
		{
		case 0:
			useWinApi = false;
			SystemLog->Out("Access mode set to 0: direct access");
			break;
		case 1:
			useWinApi = true;
			SystemLog->Out("Access mode set to 1: windows api");
			break;
		}
	}
	
	static bool CheckExitCondition(int interval, const std::string& message)
	{
		checkInterval = interval;
		auto now = std::chrono::steady_clock::now();
		if (now > nextCheck)
		{
			char exitFlag = Memory::Read<char>(Flags::inGame);
			if (exitFlag == 0)
			{
				SystemLog->Out(message, 3);
				return true;
			}
			else
			{
				nextCheck = now + std::chrono::seconds(checkInterval);
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	static DWORD GetPointerAddress(DWORD ptr, std::vector<DWORD>& offsets)  // traverses multilevel pointers
	{
		DWORD address = ptr;
		for (DWORD i = 0; i < offsets.size(); ++i)
		{
			address = *(DWORD*)address;
			address += offsets[i];
		}
		return address;
	}

	template <typename T>

	static T Read(const std::uintptr_t address, bool overrideProtection = false)
	{
		if (overrideProtection == true)
		{
			::VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &dummyOldProtection);
		}

		T value = {};

		if (useWinApi)
		{
			::ReadProcessMemory(pHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), 0);
		}
		else
		{
			value = *reinterpret_cast<const T*>(address);
		}

		return value;
	}

	
	template <typename T>

	static void Write(const std::uintptr_t address, const T& value, bool overrideProtection = false)
	{
		if (overrideProtection == true)
		{
			::VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &dummyOldProtection);
		}

		if (useWinApi)
		{
			::WriteProcessMemory(pHandle, reinterpret_cast<void*>(address), &value, sizeof(T), 0);
		}
		else
		{
			*reinterpret_cast<T*>(address) = value;
		}
	}

	// hardcoded for now :(
	static void RestoreAll()
	{
		Memory::Write(Reticle::range, 200.0f, true);

		VECTOR_3D defaultGravity{};
		defaultGravity.x = 0.0f;
		defaultGravity.y = -9.8f;
		defaultGravity.z = 0.0f;
		Memory::Write(Environment::gravity, defaultGravity, true);

		Memory::Write(Satellite::minZoom, 2.0f, true);

		Memory::Write(Satellite::maxZoom, 8.0f, true);
	}
};