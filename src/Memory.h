#pragma once

#include "Log.h"
#include <Windows.h>
#include <vector>
#include <array>

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
		if (overrideProtection = true)
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
		if (overrideProtection = true)
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

};

