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
	// there's probably a better way to do this, but oh well.
// converts the inputted key into a virtual key for windows
	int GetKeyCode(const std::string& key)
	{
		if (key == "0")
		{
			return 0x30;
		}
		else if (key == "1")
		{
			return 0x31;
		}
		else if (key == "2")
		{
			return 0x32;
		}
		else if (key == "3")
		{
			return 0x33;
		}
		else if (key == "4")
		{
			return 0x34;
		}
		else if (key == "5")
		{
			return 0x35;
		}
		else if (key == "6")
		{
			return 0x36;
		}
		else if (key == "7")
		{
			return 0x37;
		}
		else if (key == "8")
		{
			return 0x38;
		}
		else if (key == "9")
		{
			return 0x39;
		}
		else if (key == "A")
		{
			return 0x41;
		}
		else if (key == "B")
		{
			return 0x42;
		}
		else if (key == "C")
		{
			return 0x43;
		}
		else if (key == "D")
		{
			return 0x44;
		}
		else if (key == "E")
		{
			return 0x45;
		}
		else if (key == "F")
		{
			return 0x46;
		}
		else if (key == "G")
		{
			return 0x47;
		}
		else if (key == "H")
		{
			return 0x48;
		}
		else if (key == "I")
		{
			return 0x49;
		}
		else if (key == "J")
		{
			return 0x4A;
		}
		else if (key == "K")
		{
			return 0x4B;
		}
		else if (key == "L")
		{
			return 0x4C;
		}
		else if (key == "M")
		{
			return 0x4D;
		}
		else if (key == "N")
		{
			return 0x4E;
		}
		else if (key == "O")
		{
			return 0x4F;
		}
		else if (key == "P")
		{
			return 0x50;
		}
		else if (key == "Q")
		{
			return 0x51;
		}
		else if (key == "R")
		{
			return 0x52;
		}
		else if (key == "S")
		{
			return 0x53;
		}
		else if (key == "T")
		{
			return 0x54;
		}
		else if (key == "U")
		{
			return 0x55;
		}
		else if (key == "V")
		{
			return 0x56;
		}
		else if (key == "W")
		{
			return 0x57;
		}
		else if (key == "X")
		{
			return 0x58;
		}
		else if (key == "Y")
		{
			return 0x59;
		}
		else if (key == "Z")
		{
			return 0x5A;
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
