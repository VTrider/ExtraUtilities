#pragma once

#include "bzr.h"
#include "Utils.h"

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
}

namespace Reticle
{
	float GetReticleAngle();
	VECTOR_3D GetReticlePos();
	float GetSmartCursorRange();
	void SetSmartCursorRange(float range);
}

namespace Satellite
{
	char GetSatState();
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
	char GetRadarState();
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
	extern bool keyPressed;
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
}

namespace Hooks
{
	typedef void(__thiscall* _SetAsUser)(GameObject* obj);
	extern _SetAsUser SetAsUser;
}
