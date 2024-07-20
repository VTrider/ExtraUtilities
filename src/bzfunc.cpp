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
#include "Memory.h"
#include "Offsets.h"

#include "SoundBuffer.h"
#include "SoundDevice.h"
#include "SoundSource.h"

extern Log* SystemLog;

// test

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
	Memory::Write(Reticle::range, 200.0f, true);

	VECTOR_3D defaultGravity{};
	defaultGravity.x = 0.0f;
	defaultGravity.y = -9.8f;
	defaultGravity.z = 0.0f;
	Memory::Write(Environment::gravity, defaultGravity, true);

	Memory::Write(Satellite::minZoom, 2.0f, true);

	Memory::Write(Satellite::maxZoom, 8.0f, true);
}

/*-----------------------
* Environment Functions *
----------------------- */

VECTOR_3D GetGravity()
{
	return Memory::Read<VECTOR_3D>(Environment::gravity);
}

void SetGravity(float x, float y, float z)
{
	VECTOR_3D value;
	value.x = x;
	value.y = y;
	value.z = z;
	Memory::Write(Environment::gravity, value, true);
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

/*---------------------
* Satellite Functions *
--------------------- */

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

/*-----------------
* Radar Functions *
----------------- */

char GetRadarState()
{
	return Memory::Read<char>(Radar::state);
}

void SetRadarState(int state)
{
	Memory::Write(Radar::state, state);
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
}

int SetDifficulty(std::string newDifficulty)
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

DWORD jmpBackOrdnanceVelocity = 0x004803D4 + 5;

DWORD jmpBackOrdnancePosition = 0x00480357 + 7;

VECTOR_3D playerVelocity;

VECTOR_3D playerPosition;

// TODO: Refactor, it's a fkn mess

MAT_3D ordnancePosition;

struct VECTOR_3D_DOUBLE
{
	double x;
	double y;
	double z;
};

double __cdecl Distance3D(VECTOR_3D_DOUBLE* a, VECTOR_3D_DOUBLE* b)
{
	return sqrt(pow((b->x - a->x), 2) + pow((b->y - a->y), 2) + pow((b->z - a->z), 2));
}

VECTOR_3D_DOUBLE playerPosDouble;
double ordnanceDistance;

unsigned char* ordnancePosBytes;

void __declspec(naked) OrdnancePositionHook()
{
	__asm
	{
		// game code
		mov ecx, [ebp+0x08]
		push ecx
		mov ecx, [ebp-0x20]

		// preserve registers
		sub esp, 0x10
		movdqu [esp], xmm0
		pushfd
		push eax

		mov eax, [esp+0x18] // +24 bytes since I pushed a bunch of stuff
		movsd xmm0, [eax+0x28]
		movsd [ordnancePosition.posit_x], xmm0
		movsd xmm0, [eax+0x30]
		movsd [ordnancePosition.posit_y], xmm0
		movsd xmm0, [eax+0x38]
		movsd [ordnancePosition.posit_z], xmm0
		
		// restore registers
		pop eax
		popfd
		movdqu xmm0, [esp]
		add esp, 0x10

		jmp [jmpBackOrdnancePosition]
	}
}

double ordPosTolerance = 5;

void __declspec(naked) OrdnanceVelocityHook()
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

		// compare distance between local player and ordnance to only apply change to player's ordnance
		push eax // preserve

		lea eax, [playerPosDouble]
		push eax 
		lea eax, [ordnancePosition.posit_x] // start of pos section of matrix 3d
		push eax 
		call Distance3D
		add esp, 0x8 // two pointers
		pop eax // restore
		movsd[ordnanceDistance], xmm0
		movsd xmm1, [ordPosTolerance]
		comisd xmm0, xmm1
		ja notPlayerOrd // jump if above the tolerance

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
	
		notPlayerOrd:

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

float velocityScalingFactor;

void EnableOrdnanceTweak(float scalingFactor)
{
	// initialize this to 0 so nothing explodes
	playerVelocity.x = 0;
	playerVelocity.y = 0;
	playerVelocity.z = 0;
	playerPosition.x = 0;
	playerPosition.y = 0;
	playerPosition.z = 0;

	velocityScalingFactor = scalingFactor;

	if (!ordnanceTweakApplied)
	{
		ordnanceTweakBytes = Hook((void*)0x004803D4, OrdnanceVelocityHook, 5);
		ordnancePosBytes = Hook((void*)0x00480357, OrdnancePositionHook, 7);
		ordnanceTweakApplied = true;
	}
}

void UpdateOrdnance(float vx, float vy, float vz, float px, float py, float pz)
{
	playerVelocity.x = vx * velocityScalingFactor;
	playerVelocity.y = vy * velocityScalingFactor;
	playerVelocity.z = vz * velocityScalingFactor;
	playerPosition.x = px;
	playerPosition.y = py;
	playerPosition.z = pz;
	playerPosDouble.x = px;
	playerPosDouble.y = py;
	playerPosDouble.z = pz;
}

DWORD updateWeaponAimWalker = 0x0060f320;

void __declspec(naked) ShotConvergenceHook()
{
	__asm
	{
		jmp [updateWeaponAimWalker] // one line of code for this patch LOL

		// game code, doesn't really matter cause we won't return
		push ebp
		mov ebp, esp
		sub esp, 0x318
	}
}

unsigned char* shotConvergenceBytes;

bool shotConvergenceApplied = false;

void EnableShotConvergence()
{
	if (!shotConvergenceApplied)
	{
		shotConvergenceBytes = Hook((void*)0x004eb590, ShotConvergenceHook, 6);
		shotConvergenceApplied = true;
	}
}

/*-------------------------
* Internal Function Hooks *
--------------------------*/

_SetAsUser SetAsUser = (_SetAsUser)0x004DB930;

