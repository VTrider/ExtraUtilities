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
#include "Log.h"
#include "Memory.h"
#include "Offsets.h"
#include "Utils.h"

#include "lua.hpp"
#include <Windows.h>

#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

extern Log* SystemLog;

// Todo: 
// - Dynamic Sunlight
// - More TRN parameters, fog, time, and music maybe?
// - More sat parameters, wasDown, gameobject under cursor, gameobject it's centered on - you
// - Who am I looking at?


// For reticle cone angle degrees/3.75 = value needed

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

	void SetSmartCursorRange(float range)
	{
		Memory::Write(Reticle::range, range, true);
	}
}


namespace Satellite
{
	char GetSatState()
	{
		char state = Memory::Read<char>(Satellite::state);
		// need to get the inverse for satellite state
		if (state == 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
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

	void SetMinSatZoom(float zoom)
	{
		Memory::Write(Satellite::minZoom, zoom, true);
	}

	float GetMaxSatZoom()
	{
		return Memory::Read<float>(Satellite::maxZoom);
	}

	void SetMaxSatZoom(float zoom)
	{
		Memory::Write(Satellite::maxZoom, zoom, true);
	}

	float GetSatZoom()
	{
		return Memory::Read<float>(Satellite::zoom);
	}

	void SetSatZoom(float zoom)
	{
		Memory::Write(Satellite::zoom, zoom);
	}
}

namespace Radar
{
	char GetRadarState()
	{
		return Memory::Read<char>(Radar::state);
	}

	void SetRadarState(int state)
	{
		Memory::Write(Radar::state, state);
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

	bool keyPressed = false;

	bool GetGameKey(int vKey) // takes the virtual key to use in GetAsyncKeyState
	{
		if (GetAsyncKeyState(vKey))
		{
			keyPressed = true;
		}
		else
		{
			keyPressed = false;
		}
		return keyPressed;
	}

	bool GetGameKey2(int v2Key)
	{
		return GetAsyncKeyState(v2Key) != 0;
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
		char difficulty = Memory::Read<char>(Misc::difficulty);
		if (difficulty == 0)
		{
			return "Very Easy";
		}
		else if (difficulty == 1)
		{
			return "Easy";
		}
		else if (difficulty == 2)
		{
			return "Medium";
		}
		else if (difficulty == 3)
		{
			return "Hard";
		}
		else if (difficulty == 4)
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
}
