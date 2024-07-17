#pragma once

#include <Windows.h>
#include <vector>

class Memory
{
private:
	static inline bool useWinApi = false;

	static HWND hWnd;
	static DWORD pid;
	static HANDLE pHandle;
	static DWORD moduleBase;

public:
	static void Init()
	{
		hWnd = FindWindowA(0, ("Battlezone 98 Redux (2.2.301)"));
		GetWindowThreadProcessId(hWnd, &pid);
		pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		moduleBase = (DWORD)GetModuleHandle("Battlezone98Redux.exe"); // need to get the base address of the game in order to deal with multilevel pointers
	}

	static DWORD GetPointerAddress(DWORD ptr, std::vector<DWORD> offsets)  // traverses multilevel pointers
	{
		DWORD address = ptr;
		for (int i = 0; i < offsets.size(); ++i)
		{
			address = *(DWORD*)address;
			address += offsets[i];
		}
		return address;
	}

	static void Read()
	{

	}

	static void Write()
	{

	}

};

