// This is where the nitty gritty functions happen

#include "bzfunc.h"
#include "lua.hpp"
#include <Windows.h>
#include "memoryapi.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include "Log.h"

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

void ResetValues()
{
	DWORD oldProtectSmartCursorRange;
	VirtualProtect((LPVOID)0x00886B20, 4, PAGE_EXECUTE_READWRITE, &oldProtectSmartCursorRange);
	*smartCursorRange = 200;

	DWORD oldProtectGravity;
	VirtualProtect((LPVOID)0x00871A80, 12, PAGE_EXECUTE_READWRITE, &oldProtectGravity);
	gravityVector->x = 0.0;
	gravityVector->y = -9.8f;
	gravityVector->z = 0.0;

	DWORD oldProtectMinZoom;
	VirtualProtect((LPVOID)0x00872400, 4, PAGE_EXECUTE_READWRITE, &oldProtectMinZoom);
	*minSatZoom = 2;

	DWORD oldProtectMaxZoom;
	VirtualProtect((LPVOID)0x008723F4, 4, PAGE_EXECUTE_READWRITE, &oldProtectMaxZoom);
	*maxSatZoom = 8;
}


/*-----------------------
* Environment Functions *
----------------------- */

VECTOR_3D* gravityVector = (VECTOR_3D*)0x00871A80;

//VECTOR_3D* GetGravity()
//{
//	return gravityVector;
//}

DWORD pid;

VECTOR_3D GetGravity()
{
	float x, y, z;
	VECTOR_3D value;
	HWND hWnd = FindWindowA(0, ("Battlezone 98 Redux (2.2.301)"));
	GetWindowThreadProcessId(hWnd, &pid);
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	ReadProcessMemory(pHandle, gravityVector, &value, sizeof(VECTOR_3D), 0);
	return value;
}

DWORD oldProtectGravity; // you need to store the old protection value for some reason

//void SetGravity(float x, float y, float z)
//{
//	VirtualProtect((LPVOID)0x00871A80, 12, PAGE_EXECUTE_READWRITE, &oldProtectGravity); // this address was in write protected memory, this hacks it to be writable
//	gravityVector->x = x;
//	gravityVector->y = y;
//	gravityVector->z = z;
//}

void SetGravity(float x, float y, float z)
{
	VECTOR_3D value;
	value.x = x;
	value.y = y;
	value.z = z;
	VirtualProtect((LPVOID)0x00871A80, 12, PAGE_EXECUTE_READWRITE, &oldProtectGravity); // this address was in write protected memory, this hacks it to be writable
	HWND hWnd = FindWindowA(0, ("Battlezone 98 Redux (2.2.301)"));
	GetWindowThreadProcessId(hWnd, &pid);
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	WriteProcessMemory(pHandle, gravityVector, &value, sizeof(VECTOR_3D), 0);
}

// Sun_Ambient ONLY, need the other two components
// Luminance* sunlight = (Luminance*)GetPointerAddress(moduleBase + 0x0261FD24, { 0x24, 0x28 });

//Luminance* GetLuminance()
//{
//	return sunlight;
//}

//void SetLuminance(float r, float g, float b)
//{
//	sunlight->r = r;
//	sunlight->g = g;
//	sunlight->b = b;
//}

//float* fogStart = (float*)GetPointerAddress(moduleBase + 0x00520EA0, { 0x8, 0x3C, 0x4, 0x0, 0x28, 0x3FC, 0x138 });
//
//float GetFogStart()
//{
//	return *fogStart;
//}
//
//void SetFogStart(float value)
//{
//	*fogStart = value;
//}

/*-------------------
* Reticle Functions *
------------------- */

float* reticleAngle = (float*)0x025CE714;

float GetReticleAngle()
{
	return *reticleAngle;
}

VECTOR_3D* reticlePos = (VECTOR_3D*)0x025CE79C; // start of the reticle struct (3 floats)

VECTOR_3D GetReticlePos()
{
	VECTOR_3D value;
	HWND hWnd = FindWindowA(0, ("Battlezone 98 Redux (2.2.301)"));
	GetWindowThreadProcessId(hWnd, &pid);
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	ReadProcessMemory(pHandle, reticlePos, &value, sizeof(VECTOR_3D), 0);
	return value;
}

float* smartCursorRange = (float*)0x00886B20;

float GetSmartCursorRange()
{
	return *smartCursorRange;
}

DWORD oldProtectSmartCursorRange;

void SetSmartCursorRange(float range)
{
	VirtualProtect((LPVOID)0x00886B20, 4, PAGE_EXECUTE_READWRITE, &oldProtectSmartCursorRange);
	*smartCursorRange = range;
}

/*---------------------
* Satellite Functions *
--------------------- */

// char* satState = (char*)0x00917AF8; // old value bugged in MP

char* satState = (char*)0x008E8F9C; // technically if you have mouse control but it works

char GetSatState()
{
	// need to get the inverse for satellite state
	if (*satState == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

VECTOR_3D* satCursorPos = (VECTOR_3D*)0x009C9194;

VECTOR_3D* GetSatCursorPos()
{
	return satCursorPos;
}

VECTOR_3D* satCamPos = (VECTOR_3D*)0x009C91B4;

VECTOR_3D* GetSatCamPos()
{
	return satCamPos;
}

VECTOR_3D* satClickPos = (VECTOR_3D*)0x009C9188;

VECTOR_3D* GetSatClickPos()
{
	return satClickPos;
}

float* satPanSpeed = (float*)0x009C91D0;

float GetSatPanSpeed()
{
	return *satPanSpeed;
}

void SetSatPanSpeed(float speed)
{
	*satPanSpeed = speed;
}

float* minSatZoom = (float*)0x00872400;

float GetMinSatZoom()
{
	return *minSatZoom;
}

DWORD oldProtectMinZoom;

void SetMinSatZoom(float zoom)
{
	VirtualProtect((LPVOID)0x00872400, 4, PAGE_EXECUTE_READWRITE, &oldProtectMinZoom);
	*minSatZoom = zoom;
}

float* maxSatZoom = (float*)0x008723F4;

float GetMaxSatZoom() 
{
	return *maxSatZoom;
}

DWORD oldProtectMaxZoom;

void SetMaxSatZoom(float zoom)
{
	VirtualProtect((LPVOID)0x008723F4, 4, PAGE_EXECUTE_READWRITE, &oldProtectMaxZoom);
	*maxSatZoom = zoom;
}

float* satZoom = (float*)0x009C91B0;

float GetSatZoom()
{
	return *satZoom;
}

void SetSatZoom(float zoom)
{
	*satZoom = zoom;
}

/*-----------------
* Radar Functions *
----------------- */

char* radarState = (char*)0x008EAAAC; // boolean value for radar state is stored at this address

char GetRadarState()
{
	return *radarState;
}

void SetRadarState(int state)
{
	*radarState = state;
}

/*--------------
* Input/Output *
---------------*/

// there's probably a better way to do this, but oh well.
// converts the inputted key into a virtual key for windows
int GetKeyCode(std::string key)
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

/*----------------
* Misc Functions *
-----------------*/

long long* steam64 = (long long*)0x0260B1D0; // 8 byte value for steam64 ID is stored at this address

const char* GetSteam64()
{
	std::string formattedSteam64 = std::to_string(*steam64);
	const char* luaSteam64 = formattedSteam64.c_str();
	return luaSteam64;
}

int GetWeaponMask()
{
	return weaponMask;
}

int* lives = (int*)0x008E8D04;

int GetLives()
{
	return *lives;
}

void SetLives(int newLives)
{
	*lives = newLives;
}

char* difficulty = (char*)0x25CFA1C;

const char* GetDifficulty()
{
	if (*difficulty == 0)
	{
		return "Very Easy";
	}
	else if (*difficulty == 1)
	{
		return "Easy";
	}
	else if (*difficulty == 2)
	{
		return "Medium";
	}
	else if (*difficulty == 3)
	{
		return "Hard";
	}
	else if (*difficulty == 4)
	{
		return "Very Hard";
	}
}

int SetDifficulty(std::string newDifficulty)
{
	if (newDifficulty == "Very Easy")
	{
		*difficulty = 0;
	}
	else if (newDifficulty == "Easy")
	{
		*difficulty = 1;
	}
	else if (newDifficulty == "Medium")
	{
		*difficulty = 2;
	}
	else if (newDifficulty == "Hard")
	{
		*difficulty = 3;
	}
	else if (newDifficulty == "Very Hard")
	{
		*difficulty = 4;
	}
	else
	{
		return 1;
	}
	return 0;
}

DWORD jmpBackOrdnanceVelocity = 0x004803D4 + 5;

VECTOR_3D playerVelocity;

VECTOR_3D playerPosition;

// TODO: distance compare between player and ordnance to only apply tweak to player's ordnance

float __cdecl Distance3D(VECTOR_3D* a, VECTOR_3D* b)
{
	return sqrt(pow((b->x - a->x), 2) + pow((b->y - a->y), 2) + pow((b->z - a->z), 2));
}

void  __declspec(naked) OrdnanceVelocityHook()
{
	__asm
	{
		// Notes: ebp-0x10 = ordnance X velocity, ebp-0xC = ordnance Y velocity, ebp-0x08 = ordnance Z velocity
		// xmm0 stores the ordnance velocity, xmm1 stores the player velocity
		// [esp+1c] or [ebp+0x20] (should probably use this) = ecx (this) ordnance pointer +b0 offset to get to double coordinates

		// preserves the state of xmm0 and xmm1 registers so it doesn't mess anything up down the line
		sub esp, 0x10 // allocate 16 bytes to the stack to make room for the SSE register
		movdqu [esp], xmm0 // push but for SSE register
		sub esp, 0x10
		movdqu [esp], xmm1
		pushfd // push flags

		// X velocity component
		movss xmm0, [ebp-0x10] // ordnance X velocity
		movss xmm1, [playerVelocity.x]
		addss xmm0, xmm1
		movss [ebp-0x10], xmm0 // puts the modified velocity back for the game to use

		// Y velocity component
		movss xmm0, [ebp-0xC]
		movss xmm1, [playerVelocity.y]
		addss xmm0, xmm1
		movss [ebp-0xC], xmm0

		// Z velocity component
		movss xmm0, [ebp-0x08]
		movss xmm1, [playerVelocity.z]
		addss xmm0, xmm1
		movss [ebp-0x08], xmm0
	
		// restore SSE registers & flags
		popfd
		movdqu xmm1, [esp]
		add esp, 0x10
		movdqu xmm0, [esp]
		add esp, 0x10
		
		// resume execution
		mov ecx, [ebp-0x10]
		mov [eax], ecx
		jmp[jmpBackOrdnanceVelocity]
	}
}

bool ordnanceTweakApplied = false;

unsigned char* ordnanceTweakBytes;

void EnableOrdnanceTweak()
{
	// initialize this to 0 so nothing explodes
	playerVelocity.x = 0;
	playerVelocity.y = 0;
	playerVelocity.z = 0;
	playerPosition.x = 0;
	playerPosition.y = 0;
	playerPosition.z = 0;

	if (!ordnanceTweakApplied)
	{
		ordnanceTweakBytes = Hook((void*)0x004803D4, OrdnanceVelocityHook, 5);
		ordnanceTweakApplied = true;
	}
}

void UpdateOrdnance(float vx, float vy, float vz, float px, float py, float pz)
{
	playerVelocity.x = vx;
	playerVelocity.y = vy;
	playerVelocity.z = vz;
	playerPosition.x = px;
	playerPosition.y = py;
	playerPosition.z = pz;
}

/*-------------------------
* Internal Function Hooks *
--------------------------*/

_SetAsUser SetAsUser = (_SetAsUser)0x004DB930;

