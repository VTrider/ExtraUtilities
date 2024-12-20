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

// This is where the nitty gritty functions happen

#include "bzfunc.h"

#include "asm.h"
#include "Hook.h"
#include "Log.h"
#include "Memory.h"
#include "Offsets.h"
#include "structs.h"

#include "lua.hpp"
#include <Windows.h>

#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/*---------------------
* Important Functions *
----------------------*/

// This function gets the internal "handle" of an object, Credit to Janne -VT
// static pointer to the player's gameobject pointer:  0x00917AFC
GameObject* GetObj(unsigned int handle)
{
	return (GameObject*)(((handle >> 0x14) * 0x400) + 0x260DB20); // wtf lol magic numbers
}

namespace Environment
{
	VECTOR_3D GetGravity()
	{
		return Memory::Read<VECTOR_3D>(Environment::gravity);
	}

	void SetGravity(float x, float y, float z)
	{
		VECTOR_3D value{};
		value.x = x;
		value.y = y;
		value.z = z;
		Memory::Write(Environment::gravity, value, true);
	}

	Fog GetFog()
	{
		return Memory::Read<Fog>(Environment::fog);
	}

	void SetFog(float r, float g, float b, float start, float ending)
	{
		Fog newFog{};
		newFog.r = r;
		newFog.g = g;
		newFog.b = b;
		newFog.start = start;
		newFog.ending = ending;
		Memory::Write(Environment::fog, newFog);
	}

	OgreColor GetSunAmbient()
	{
		return Memory::Read<OgreColor>(Environment::sunAmbient);
	}

	void SetSunAmbient(float r, float g, float b)
	{
		Memory::Write(Environment::sunAmbient, OgreColor{ r, g, b });
	}
}

namespace Reticle
{
	float GetReticleAngle()
	{
		return Memory::Read<float>(Reticle::angle);
	}

	VECTOR_3D GetReticlePos()
	{
		return Memory::Read<VECTOR_3D>(Reticle::position);
	}
	
	float GetSmartCursorRange()
	{
		return Memory::Read<float>(Reticle::range);
	}

	void SetSmartCursorRange(float newRange)
	{
		Memory::Write(Reticle::range, newRange, true);
	}

	void* GetReticleObject()
	{
		return Memory::Read<void*>(Reticle::waila);
	}

	MAT_3D GetReticleMatrix()
	{
		return Memory::Read<MAT_3D>(Reticle::matrix);
	}
}

namespace Satellite
{
	bool GetSatState()
	{
		bool satelliteState = Memory::Read<bool>(Satellite::state);
		return satelliteState ? false : true; // need to get the inverse for satellite state
	}

	VECTOR_3D GetSatCursorPos()
	{
		return Memory::Read<VECTOR_3D>(Satellite::cursorPos);
	}

	VECTOR_3D GetSatCamPos()
	{
		return Memory::Read<VECTOR_3D>(Satellite::camPos);
	}

	VECTOR_3D GetSatClickPos()
	{
		return Memory::Read<VECTOR_3D>(Satellite::clickPos);
	}

	float GetSatPanSpeed()
	{
		return Memory::Read<float>(Satellite::panSpeed);
	}

	void SetSatPanSpeed(float speed)
	{
		Memory::Write(Satellite::panSpeed, speed);
	}

	float GetMinSatZoom()
	{
		return Memory::Read<float>(Satellite::minZoom);
	}

	void SetMinSatZoom(float newZoom)
	{
		Memory::Write(Satellite::minZoom, newZoom, true);
	}

	float GetMaxSatZoom()
	{
		return Memory::Read<float>(Satellite::maxZoom);
	}

	void SetMaxSatZoom(float newZoom)
	{
		Memory::Write(Satellite::maxZoom, newZoom, true);
	}

	float GetSatZoom()
	{
		return Memory::Read<float>(Satellite::zoom);
	}

	void SetSatZoom(float newZoom)
	{
		Memory::Write(Satellite::zoom, newZoom);
	}
}

namespace Radar
{
	int GetRadarState()
	{
		return Memory::Read<int>(Radar::state);
	}

	void SetRadarState(int newState)
	{
		Memory::Write(Radar::state, newState);
	}
}

namespace Camera
{
	float GetZoomFactor(const std::string& camera)
	{
		if (camera == "F")
		{
			return Memory::Read<float>(Camera::zoomFactorFPP);
		}
		else if (camera == "G")
		{
			return Memory::Read<float>(Camera::zoomFactorGlobal);
		}
		else
		{
			return -1.0f;
		}
	}

	void SetZoomFactor(float factor, const std::string& camera)
	{
		if (camera == "F")
		{
			Memory::Write(Camera::zoomFactorFPP, factor);
		}
		else if (camera == "G")
		{
			Memory::Write(Camera::zoomFactorGlobal, factor);
		}
		else
		{
			return;
		}
	}

	float GetMinZoomFactor()
	{
		return Memory::Read<float>(Camera::minZoomFactor);
	}

	void SetMinZoomFactor(float factor)
	{
		Memory::Write(Camera::minZoomFactor, factor);
	}

	float GetMaxZoomFactor()
	{
		return Memory::Read<float>(Camera::maxZoomFactor);
	}

	void SetMaxZoomFactor(float factor)
	{
		Memory::Write(Camera::maxZoomFactor, factor);
	}

	Frustum GetViewFrustum()
	{
		return Memory::Read<Frustum>(Camera::viewFrustum);
	}
}

namespace IO
{
	int GetKeyCode(const std::string& key)
	{
		// this is still pretty aids but a lot better than if else hell lol
		static const std::unordered_map<std::string, int> keyMap = {
			{"0", 0x30}, {"1", 0x31}, {"2", 0x32}, {"3", 0x33}, {"4", 0x34},
			{"5", 0x35}, {"6", 0x36}, {"7", 0x37}, {"8", 0x38}, {"9", 0x39},
			{"A", 0x41}, {"B", 0x42}, {"C", 0x43}, {"D", 0x44}, {"E", 0x45},
			{"F", 0x46}, {"G", 0x47}, {"H", 0x48}, {"I", 0x49}, {"J", 0x4A},
			{"K", 0x4B}, {"L", 0x4C}, {"M", 0x4D}, {"N", 0x4E}, {"O", 0x4F},
			{"P", 0x50}, {"Q", 0x51}, {"R", 0x52}, {"S", 0x53}, {"T", 0x54},
			{"U", 0x55}, {"V", 0x56}, {"W", 0x57}, {"X", 0x58}, {"Y", 0x59},
			{"Z", 0x5A}, {"CTRL", VK_CONTROL}, {"LCTRL", VK_LCONTROL}, {"RCTRL", VK_RCONTROL},
			{"ALT", VK_MENU}, {"LALT", VK_LMENU}, {"RALT", VK_RMENU}, {"SHIFT", VK_SHIFT},
			{"LSHIFT", VK_LSHIFT}, {"RSHIFT", VK_RSHIFT}, {"CAPSLOCK", VK_CAPITAL},
			{"TAB", VK_TAB}, {"PAUSE", VK_PAUSE}, {"PAGEUP", VK_PRIOR}, {"PAGEDOWN", VK_NEXT},
			{"END", VK_END}, {"HOME", VK_HOME}, {"INSERT", VK_INSERT}, {"DELETE", VK_DELETE},
			{"BACKSPACE", VK_BACK}, {"ENTER", VK_RETURN}, {"SPACE", VK_SPACE},
			{"LMB", VK_LBUTTON}, {"RMB", VK_RBUTTON}, {"MMB", VK_MBUTTON}, {"XB1", VK_XBUTTON1},
			{"XB2", VK_XBUTTON2}, {"ESCAPE", VK_ESCAPE}, {"LARROW", VK_LEFT}, {"UARROW", VK_UP},
			{"RARROW", VK_RIGHT}, {"DARROW", VK_DOWN}, {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3},
			{"F4", VK_F4}, {"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8},
			{"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
		};

		auto it = keyMap.find(key);
		if (it != keyMap.end())
		{
			return it->second;
		}
		else
		{
			return -1;
		}
	}

	bool GetGameKey(int vKey) // takes the virtual key to use in GetAsyncKeyState
	{
		return GetAsyncKeyState(vKey) ? true : false;
	}
}

namespace Misc
{
	const char* GetSteam64()
	{
		std::string formattedSteam64 = std::to_string(Memory::Read<long long>(Misc::steam64));
		const char* luaSteam64 = formattedSteam64.c_str();
		return luaSteam64;
	}

	int GetWeaponMask()
	{
		return weaponMask;
	}

	int GetLives()
	{
		return Memory::Read<int>(Misc::lives);
	}

	void SetLives(int newLives)
	{
		Memory::Write(Misc::lives, newLives);
	}

	const char* GetDifficulty()
	{
		char currentDifficulty = Memory::Read<char>(Misc::difficulty);
		if (currentDifficulty == 0)
		{
			return "Very Easy";
		}
		else if (currentDifficulty == 1)
		{
			return "Easy";
		}
		else if (currentDifficulty == 2)
		{
			return "Medium";
		}
		else if (currentDifficulty == 3)
		{
			return "Hard";
		}
		else if (currentDifficulty == 4)
		{
			return "Very Hard";
		}
		else
		{
			return "Invalid Input";
		}
	}

	int SetDifficulty(const std::string& newDifficulty)
	{
		if (newDifficulty == "Very Easy")
		{
			Memory::Write(Misc::difficulty, 0);
		}
		else if (newDifficulty == "Easy")
		{
			Memory::Write(Misc::difficulty, 1);
		}
		else if (newDifficulty == "Medium")
		{
			Memory::Write(Misc::difficulty, 2);
		}
		else if (newDifficulty == "Hard")
		{
			Memory::Write(Misc::difficulty, 3);
		}
		else if (newDifficulty == "Very Hard")
		{
			Memory::Write(Misc::difficulty, 4);
		}
		else
		{
			return 1;
		}
		return 0;
	}

	float GetCoeffMortar()
	{
		return Memory::Read<float>(Misc::coeffMortar, true);
	}

	void SetCoeffMortar(float newCoeff)
	{
		Memory::Write(Misc::coeffMortar, newCoeff, true);
	}

	bool GetPlayerWeaponMask(int hardpoint)
	{
		return (Memory::Read<int>(Misc::playerWeapons) >> (hardpoint)) & 1;
	}

	void SetPlayerWeaponMask(int hardpoint, bool setting)
	{
		int weapons = Memory::Read<int>(Misc::playerWeapons);
		if (setting == true)
		{
			Memory::Write(Misc::playerWeapons, weapons |= (1 << (hardpoint)));
		}
		else
		{
			Memory::Write(Misc::playerWeapons, weapons &= (~(1 << (hardpoint))));
		}
	}

	float GetScrapMultiplier()
	{
		return Misc::scrapMultiplier;
	}

	void SetScrapMultiplier(float multiplier)
	{
		Misc::scrapMultiplier = multiplier;
	}

	// The magic numbers in these functions correspond to the x86 opcodes for JNA (jump not above): 0x66,
	// and JA (jump above) 0x67.

	bool GetGlobalTurbo()
	{
		return (Memory::Read<std::uint8_t>(Misc::turbo, true) == 0x67) ? true : false;
	}

	// For some reason writing to memory using Memory::Write or just raw pointer
	// dereference would corrupt the machine code, no idea why because this does
	// seemingly the same thing. This function is voodoo but pretty much it just
	// bypasses the various conditions that stop units from using turbo speed.
	void SetGlobalTurbo(bool newTurbo)
	{
		static bool doneInit = false;
		static float tolerance = 1.0f;

		if (!doneInit)
		{
			DWORD temp;
			VirtualProtect((LPVOID)Misc::turbo, 2, PAGE_EXECUTE_READWRITE, &temp);
			// address of the target address of the comiss opcode
			VirtualProtect((LPVOID)0x00601CA3, 4, PAGE_EXECUTE_READWRITE, &temp);

			// save all these bytes cause we're gonna clobber them
			unsigned char* turboBytes = new unsigned char[2];
			memcpy(turboBytes, (void*)Misc::turbo, 2);
			unsigned char* comissBytes = new unsigned char[4];
			memcpy(comissBytes, (void*)0x00601CA3, 4);

			Hook::AddOtherRestore({ Misc::turbo, turboBytes, 2 });
			Hook::AddOtherRestore({ 0x00601CA3, comissBytes, 4 });

			auto p_tolerance = &tolerance;
			memcpy((void*)0x00601CA3, &p_tolerance, 4);

			doneInit = true;
		}
		
		static std::uint8_t origBytes[2] = { 0x76, 0x0C }; // JNA
		static std::uint8_t nopBytes[2] = { 0x90, 0x90 }; // NOP (x2)
		
		// actually set the values/patch stuff
		tolerance = (newTurbo == true) ? 0.9f : 1.0f;
		memcpy((void*)Misc::turbo, (newTurbo == true) ? nopBytes : origBytes, 2);
	}
}
