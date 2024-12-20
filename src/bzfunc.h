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
#include "structs.h"

#include <Windows.h>

#include <string>
#include <vector>

/*---------------------
* Important Functions *
----------------------*/

GameObject* GetObj(unsigned int handle);

namespace Environment
{
	VECTOR_3D GetGravity();
	void SetGravity(float x, float y, float z);
	Fog GetFog();
	void SetFog(float r, float g, float b, float start, float ending);
	OgreColor GetSunAmbient();
	void SetSunAmbient(float r, float g, float b);
}

namespace Reticle
{
	float GetReticleAngle();
	VECTOR_3D GetReticlePos();
	float GetSmartCursorRange();
	void SetSmartCursorRange(float range);
	void* GetReticleObject();
	MAT_3D GetReticleMatrix();
}

namespace Satellite
{
	bool GetSatState();
	VECTOR_3D GetSatCursorPos();
	VECTOR_3D GetSatCamPos();
	VECTOR_3D GetSatClickPos();
	float GetSatPanSpeed();
	void SetSatPanSpeed(float speed);
	float GetMinSatZoom();
	void SetMinSatZoom(float zoom);
	float GetMaxSatZoom();
	void SetMaxSatZoom(float zoom);
	float GetSatZoom();
	void SetSatZoom(float zoom);
}

namespace Radar
{
	int GetRadarState();
	void SetRadarState(int state);
}

namespace Camera
{
	float GetZoomFactor(const std::string& camera);
	void SetZoomFactor(float factor, const std::string& camera);
	float GetMinZoomFactor();
	void SetMinZoomFactor(float factor);
	float GetMaxZoomFactor();
	void SetMaxZoomFactor(float factor);
	Frustum GetViewFrustum();
}

namespace IO
{
	int GetKeyCode(const std::string& keycode);
	bool GetGameKey(int key);
}

namespace Misc
{
	const char* GetSteam64();
	int GetWeaponMask();
	int GetLives();
	void SetLives(int newLives);
	const char* GetDifficulty();
	int SetDifficulty(const std::string& newDifficulty);
	float GetCoeffMortar();
	void SetCoeffMortar(float newCoeff);
	bool GetPlayerWeaponMask(int hardpoint);
	void SetPlayerWeaponMask(int hardpoint, bool setting);
	extern float scrapMultiplier;
	float GetScrapMultiplier();
	void SetScrapMultiplier(float multiplier);
	bool GetGlobalTurbo();
	void SetGlobalTurbo(bool turbo);
}