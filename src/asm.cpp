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

// Assembly code

#include "asm.h"

#include "exumeta.h"
#include "Hook.h"
#include "Log.h"
#include "Offsets.h"
#include "structs.h"

#include <sol/sol.hpp>

#include <memory>
#include <stdio.h>
#include <string.h>

std::uint32_t Misc::weaponMask;

std::uint32_t jmpBackWeaponMask = static_cast<std::uint32_t>(Hooks::weaponMask) + 9; // this index is hardcoded for now unfortunately

void  __declspec(naked) WeaponMaskHook()
{
    __asm
    {
        // the value of the player's weapon mask is in the edx register at 
        // this location, this moves it into a variable before it executes 
        // the rest of the code normally
        mov[Misc::weaponMask], edx
        mov[ecx + 0x1C], edx
        mov eax, [ebp - 0x00000110]
        jmp[jmpBackWeaponMask] // jumps back to the original code to resume normal execution
    }
}

bool ordnanceTweakApplied = false;

VECTOR_3D playerVelocity;
VECTOR_3D playerPosition;
VECTOR_3D_DOUBLE playerPosDouble;
MAT_3D ordnancePosition;
double ordnanceDistance;
double ordPosTolerance = 5;

static double __cdecl Distance3D(VECTOR_3D_DOUBLE* a, VECTOR_3D_DOUBLE* b)
{
	return sqrt(pow((b->x - a->x), 2) + pow((b->y - a->y), 2) + pow((b->z - a->z), 2));
}

std::uint32_t jmpBackOrdnanceVelocity = static_cast<std::uint32_t>(Hooks::ordnanceVelocity) + 5;

void __declspec(naked) OrdnanceVelocityHook()
{
	__asm
	{
		// Notes: ebp-0x10 = ordnance X velocity, ebp-0xC = ordnance Y velocity, ebp-0x08 = ordnance Z velocity
		// xmm0 stores the ordnance velocity, xmm1 stores the player velocity
		// [esp+1c] or [ebp+0x20] (should probably use this) = ecx (this) ordnance pointer +b0 offset to get to double coordinates

		// check if patch is enabled
		cmp [ordnanceTweakApplied], 0x1
		jne [notEnabled]

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
		
		notEnabled:
		// resume execution
		mov ecx, [ebp-0x10]
		mov [eax], ecx
		jmp[jmpBackOrdnanceVelocity]
	}
}

std::uint32_t jmpBackOrdnancePosition = static_cast<std::uint32_t>(Hooks::ordnancePosition) + 7;

void __declspec(naked) OrdnancePositionHook()
{
	__asm
	{
		// game code
		mov ecx, [ebp + 0x08]
		push ecx
		mov ecx, [ebp - 0x20]

		// check if patch is enabled
		cmp[ordnanceTweakApplied], 0x1
		jne[notEnabled]

		// preserve registers
		sub esp, 0x10
		movdqu[esp], xmm0
		pushfd
		push eax

		mov eax, [esp + 0x18] // +24 bytes since I pushed a bunch of stuff
		movsd xmm0, [eax + 0x28]
		movsd[ordnancePosition.posit_x], xmm0
		movsd xmm0, [eax + 0x30]
		movsd[ordnancePosition.posit_y], xmm0
		movsd xmm0, [eax + 0x38]
		movsd[ordnancePosition.posit_z], xmm0

		// restore registers
		pop eax
		popfd
		movdqu xmm0, [esp]
		add esp, 0x10

		notEnabled:

		jmp[jmpBackOrdnancePosition]
	}
}

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

bool shotConvergenceApplied = false;

std::uint32_t updateWeaponAimWalker = 0x0060f320;

std::uint32_t jmpBackHovercraft = static_cast<std::uint32_t>(Hooks::shotConvergence) + 9;

void __declspec(naked) ShotConvergenceHook()
{
	__asm
	{
		// check if patch is enabled
		cmp[shotConvergenceApplied], 0x1
		jne[notEnabled]

		jmp[updateWeaponAimWalker] // one line of code for this patch LOL

		notEnabled:

		push ebp
		mov ebp, esp
		sub esp, 0x318
		jmp [jmpBackHovercraft]
	}
}

void EnableShotConvergence()
{
	if (!shotConvergenceApplied)
	{
		shotConvergenceApplied = true;
	}
}

std::uint32_t jmpBackLightPtr = static_cast<std::uint32_t>(Hooks::getLightPtr) + 6;

void* lightObj;

const char* label;

static void __cdecl AddToUnitLights()
{
	UnitLight thisUnit{};

	auto light = std::make_unique<void*>(lightObj);

	thisUnit.label = label;
	thisUnit.light = std::move(light);

	Hook::unitLights.push_back(std::move(thisUnit));
}

void __declspec(naked) LightPtrHook()
{
	__asm
	{
		
		push eax
		push edx

		lea eax, [ecx]
		mov [lightObj], eax

		lea eax, [ebp + 0xE0]
		mov [label], eax

		call AddToUnitLights

		pop edx
		pop eax

		mov ecx, [ebp - 0xE8]
		jmp [jmpBackLightPtr]
	}
}

bool disableSelectNone = false;
std::uint32_t jmpBackSelectNone = static_cast<std::uint32_t>(Hooks::selectNone) + 6;
std::uint32_t selectNoneRet = 0x004A6DC5;

void __declspec(naked) SelectNoneHook()
{
	__asm
	{
		cmp [disableSelectNone], 0x1
		jne [notEnabled]

		// if disabled select none is true then jump to the end of the function
		// and immediately return
		jmp [selectNoneRet]

		notEnabled:

		push ebp
		mov ebp, esp
		sub esp, 0xC
		jmp [jmpBackSelectNone]
	}
}

void SetSelectNone(bool setting)
{
	disableSelectNone = setting;
}

static void __cdecl lua_BulletHit(const char* odf, int* obj, VECTOR_3D_DOUBLE* position)
{
	auto exu_callback = lua->get<sol::table>("exu_callback");
	auto BulletHit = exu_callback.get<sol::function>("BulletHit");
	auto SetVector = lua->get<sol::function>("SetVector");

	auto positionVector = SetVector(static_cast<float>(position->x), static_cast<float>(position->y), static_cast<float>(position->z));

	void* handle;

	if (*obj == 0)
	{
		handle = nullptr;
	}
	else
	{
		handle = reinterpret_cast<void*>(FuncPtrs::GetHandle(*obj));
	}

	BulletHit(odf, handle, positionVector);
}

std::uint32_t jmpBackBulletHit = static_cast<std::uint32_t>(Hooks::bulletHitCB) + 7;

// eax+0c for odf, ebp+08 for gameobject*, ecx+48 for pos

void __declspec(naked) BulletHitCallback()
{
	__asm
	{
		pushad
		pushfd

		lea ebx, [eax] // preserve eax

		lea eax, [ecx+0x48] // vec3 double position
		push eax

		lea eax, [ebp+0x08] // gameobject* of hit object if it exists
		push eax

		mov eax, [ebx+0x0C] // ordnance*
		lea ebx, [eax+0x20] // odf char*
		push ebx

		call lua_BulletHit
		add esp, 0x0C

		popfd
		popad

		add ecx, 0x38
		push ecx
		mov edx, [ebp+0x0C]

		jmp [jmpBackBulletHit]
	}
}