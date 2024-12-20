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

#include <bzr.h>
#include "Log.h"
#include "Offsets.h"

#include <Windows.h>

#include <format>
#include <optional>
#include <variant>
#include <vector>

void LockCursor();

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
	// hardcoded at 5 seconds cause there's a race condition between this
	// value and the dll reading it, so 0 or "right away" could potentially close
	// the thread upon entering the game
	static inline auto nextCheck = std::chrono::steady_clock::now() + std::chrono::seconds(5);

	// Ogre functions are at a weird offset that needs to be accounted for.
	static std::uintptr_t GetOgreFunction(LPCSTR mangledName)
	{
		char* jmpAddress = reinterpret_cast<char*>(GetProcAddress(ogreMain, mangledName));
		std::int32_t offset = *reinterpret_cast<std::int32_t*>(jmpAddress + 1);
		std::uintptr_t realAddress = reinterpret_cast<std::uintptr_t>(jmpAddress + 5 + offset);
		return realAddress;
	}

public:

	static inline HMODULE ogreMain{};

	static void Init()
	{
		hWnd = FindWindowA(0, ("Battlezone 98 Redux (2.2.301)"));
		GetWindowThreadProcessId(hWnd, &pid);
		pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		moduleBase = (DWORD)GetModuleHandle("Battlezone98Redux.exe"); // need to get the base address of the game in order to deal with multilevel pointers

		ogreMain = GetModuleHandle("OgreMain.dll");

		DefineOgreFunctions();
	}

	static void DefineOgreFunctions();

	static void SetAccessMode(int mode)
	{
		switch (mode)
		{
		case 0:
			useWinApi = false;
			break;
		case 1:
			useWinApi = true;
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
				auto temp = message;
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

	static std::uintptr_t GetPointerAddress(std::uint32_t ptr, std::vector<std::uint32_t> offsets)  // traverses multilevel pointers
	{
		std::uint32_t address = ptr;
		for (std::size_t i = 0; i < offsets.size(); ++i)
		{
			address = *(std::uint32_t*)address;
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

		Memory::Write(Misc::coeffMortar, 4.9f, true);

		LockCursor();
	}
};