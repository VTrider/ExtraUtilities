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

/*----------------------------------
* Exported lua functions go here,  *
* sends/receieves data to/from the *
* lua stack						   *
-----------------------------------*/

#pragma warning(disable : 4731) // warning about modifying the stack frame in the assembly code, it's necessary here

#include "asm.h"
#include "bzfunc.h"
#include "exumeta.h"
#include "filesystem.h"
#include "Hook.h"
#include "Memory.h"
#include "Offsets.h"

#include "lua.hpp"
#include <Windows.h>

#include <chrono>
#include <thread>
#include <vector>
#include "Log.h"

#pragma region IMPORTANT_FUNCTIONS

static int lua_GetVersion(lua_State* L)
{
	lua_pushstring(L, Exu::version.c_str());
	return 1;
}

static int lua_SetAccessMode(lua_State* L)
{
	int mode = luaL_checkinteger(L, 1);
	Memory::SetAccessMode(mode);
	return 0;
}

static int lua_GetObj(lua_State* L)
{
	void* handle = (void*)lua_touserdata(L, 1);
	GameObject* gameObject = GetObj((unsigned int)handle);
	lua_pushlightuserdata(L, gameObject);
	return 1;
}

#pragma endregion IMPORTANT_FUNCTIONS

#pragma region ENVIRONMENT

static int lua_GetGravity(lua_State* L)
{
	VECTOR_3D gravity = Environment::GetGravity();
	lua_createtable(L, 0, 3);

	lua_pushnumber(L, gravity.x);
	lua_setfield(L, -2, "x");

	lua_pushnumber(L, gravity.y);
	lua_setfield(L, -2, "y");

	lua_pushnumber(L, gravity.z);
	lua_setfield(L, -2, "z");

	return 1;
}

int	lua_SetGravity(lua_State* L)
{
	float x = static_cast<float>(luaL_checknumber(L, 1));
	float y = static_cast<float>(luaL_checknumber(L, 2));
	float z = static_cast<float>(luaL_checknumber(L, 3));
	Environment::SetGravity(x, y, z);
	return 0;
}

#pragma endregion ENVIRONMENT

#pragma region RETICLE

static int lua_GetReticleAngle(lua_State* L)
{
	lua_pushnumber(L, Reticle::GetReticleAngle());
	return 1;
}

static int lua_GetReticlePos(lua_State* L) 
{
	try
	{
		VECTOR_3D reticlePos = Reticle::GetReticlePos();
		lua_createtable(L, 0, 3);

		lua_pushnumber(L, reticlePos.x);
		lua_setfield(L, -2, "x");

		lua_pushnumber(L, reticlePos.y);
		lua_setfield(L, -2, "y");

		lua_pushnumber(L, reticlePos.z);
		lua_setfield(L, -2, "z");

		return 1;
	}
	catch (const std::exception& e)
	{
		SystemLog->Out("[EXU ERROR]: " + std::string(e.what()));
		return 0;
	}
	catch (...)
	{
		SystemLog->Out("[EXU ERROR]: Caught unidentified exception, oh whale!");
		return 0;
	}
}

static int lua_GetSmartCursorRange(lua_State* L)
{
	lua_pushnumber(L, Reticle::GetSmartCursorRange());
	return 1;
}

static int lua_SetSmartCursorRange(lua_State* L)
{
	try
	{
		float range = static_cast<float>(luaL_checknumber(L, 1));
		Reticle::SetSmartCursorRange(range);
		return 0;
	}
	catch (const std::exception& e)
	{
		SystemLog->Out("[EXU ERROR]: " + std::string(e.what()));
		return 0;
	}
	catch (...)
	{
		SystemLog->Out("[EXU ERROR]: Caught unidentified exception, oh whale!");
		return 0;
	}
}

#pragma endregion RETICLE

#pragma region SATELLITE

static int lua_GetSatState(lua_State* L)
{
	try
	{
		lua_pushnumber(L, Satellite::GetSatState());
		return 1;
	}
	catch (const std::exception& e)
	{
		SystemLog->Out("[EXU ERROR]: " + std::string(e.what()));
		return 0;
	}
	catch (...)
	{
		SystemLog->Out("[EXU ERROR]: Caught unidentified exception, oh whale!");
		return 0;
	}
}

static int lua_GetSatCursorPos(lua_State* L)
{
	try
	{
		VECTOR_3D cursorPos = Satellite::GetSatCursorPos();
		lua_createtable(L, 0, 3);

		lua_pushnumber(L, cursorPos.x);
		lua_setfield(L, -2, "x");

		lua_pushnumber(L, cursorPos.y);
		lua_setfield(L, -2, "y");

		lua_pushnumber(L, cursorPos.z);
		lua_setfield(L, -2, "z");

		return 1;
	}
	catch (const std::exception& e)
	{
		SystemLog->Out("[EXU ERROR]: " + std::string(e.what()));
		return 0;
	}
	catch (...)
	{
		SystemLog->Out("[EXU ERROR]: Caught unidentified exception, oh whale!");
		return 0;
	}
}

static int lua_GetSatCamPos(lua_State* L)
{
	VECTOR_3D camPos = Satellite::GetSatCamPos();
	lua_createtable(L, 0, 3);

	lua_pushnumber(L, camPos.x);
	lua_setfield(L, -2, "x");

	lua_pushnumber(L, camPos.y);
	lua_setfield(L, -2, "y");

	lua_pushnumber(L, camPos.z);
	lua_setfield(L, -2, "z");

	return 1;
}

static int lua_GetSatClickPos(lua_State* L)
{
	VECTOR_3D clickPos = Satellite::GetSatClickPos();
	lua_createtable(L, 0, 3);

	lua_pushnumber(L, clickPos.x);
	lua_setfield(L, -2, "x");

	lua_pushnumber(L, clickPos.y);
	lua_setfield(L, -2, "y");

	lua_pushnumber(L, clickPos.z);
	lua_setfield(L, -2, "z");

	return 1;
}

static int lua_GetSatPanSpeed(lua_State* L)
{
	lua_pushnumber(L, Satellite::GetSatPanSpeed());
	return 1;
}

static int lua_SetSatPanSpeed(lua_State* L)
{
	try
	{
		float speed = static_cast<float>(luaL_checknumber(L, 1));
		Satellite::SetSatPanSpeed(speed);
		return 0;
	}
	catch (const std::exception& e)
	{
		SystemLog->Out("[EXU ERROR]: " + std::string(e.what()));
		return 0;
	}
	catch (...)
	{
		SystemLog->Out("[EXU ERROR]: Caught unidentified exception, oh whale!");
		return 0;
	}
}

static int lua_GetMinSatZoom(lua_State* L)
{
	lua_pushnumber(L, Satellite::GetMinSatZoom());
	return 1;
}

static int lua_SetMinSatZoom(lua_State* L)
{
	float zoom = static_cast<float>(luaL_checknumber(L, 1));
	Satellite::SetMinSatZoom(zoom);
	return 0;
}

static int lua_GetMaxSatZoom(lua_State* L)
{
	lua_pushnumber(L, Satellite::GetMaxSatZoom());
	return 1;
}

static int lua_SetMaxSatZoom(lua_State* L)
{
	float zoom = static_cast<float>(luaL_checknumber(L, 1));
	Satellite::SetMaxSatZoom(zoom);
	return 0;
}

static int lua_GetSatZoom(lua_State* L)
{
	lua_pushnumber(L, Satellite::GetSatZoom());
	return 1;
}

static int lua_SetSatZoom(lua_State* L)
{
	try
	{
		float zoom = static_cast<float>(luaL_checknumber(L, 1));
		Satellite::SetSatZoom(zoom);
		return 0;
	}
	catch (const std::exception& e)
	{
		SystemLog->Out("[EXU ERROR]: " + std::string(e.what()));
		return 0;
	}
	catch (...)
	{
		SystemLog->Out("[EXU ERROR]: Caught unidentified exception, oh whale!");
		return 0;
	}
}

#pragma endregion SATELLITE

#pragma region RADAR

/*-----------------
* Radar Functions *
------------------*/

static int lua_GetRadarState(lua_State* L) 
{
	lua_pushnumber(L, Radar::GetRadarState());
	return 1;
}

static int lua_SetRadarState(lua_State* L) 
{
	int state = luaL_checkinteger(L, 1);
	Radar::SetRadarState(state);
	return 0;
}

#pragma endregion RADAR

#pragma region CAMERA

static int lua_GetZoomFactor(lua_State* L)
{
	std::string camera = luaL_checkstring(L, 1);
	lua_pushnumber(L, Camera::GetZoomFactor(camera));
	return 1;
}

static int lua_SetZoomFactor(lua_State* L)
{
	float factor = static_cast<float>(luaL_checknumber(L, 1));
	std::string camera = luaL_checkstring(L, 2);
	Camera::SetZoomFactor(factor, camera);
	return 0;
}

static int lua_GetMinZoomFactor(lua_State* L)
{
	lua_pushnumber(L, Camera::GetMinZoomFactor());
	return 1;
}

static int lua_SetMinZoomFactor(lua_State* L)
{
	float factor = static_cast<float>(luaL_checknumber(L, 1));
	Camera::SetMinZoomFactor(factor);
	return 0;
}

static int lua_GetMaxZoomFactor(lua_State* L)
{
	lua_pushnumber(L, Camera::GetMaxZoomFactor());
	return 1;
}

static int lua_SetMaxZoomFactor(lua_State* L)
{
	float factor = static_cast<float>(luaL_checknumber(L, 1));
	Camera::SetMaxZoomFactor(factor);
	return 0;
}

#pragma endregion CAMERA

#pragma region IO

static int lua_GetGameKey(lua_State* L)
{
	std::string key = luaL_checkstring(L, 1);
	int vKey = (IO::GetKeyCode(key));
	lua_pushboolean(L, IO::GetGameKey(vKey));
	return 1;
}

#pragma endregion IO

#pragma region MISC

static int lua_GetSteam64(lua_State* L) 
{
	try
	{
		lua_pushstring(L, Misc::GetSteam64());
		return 1;
	}
	catch (const std::exception& e)
	{
		SystemLog->Out("[EXU ERROR]: " + std::string(e.what()));
		return 0;
	}
	catch (...)
	{
		SystemLog->Out("[EXU ERROR]: Caught unidentified exception, oh whale!");
		return 0;
	}
}

static int lua_GetWeaponMask(lua_State* L)
{
	lua_pushnumber(L, Misc::GetWeaponMask());
	return 1;
}

static int lua_GetLives(lua_State* L)
{
	lua_pushnumber(L, Misc::GetLives());
	return 1;
}

static int lua_SetLives(lua_State* L)
{
	int newLives = luaL_checkinteger(L, 1);
	Misc::SetLives(newLives);
	return 1;
}

static int lua_GetDifficulty(lua_State* L)
{
	lua_pushstring(L, Misc::GetDifficulty());
	return 1;
}

static int lua_SetDifficulty(lua_State* L)
{
	std::string newDifficulty = luaL_checkstring(L, 1);
	if (Misc::SetDifficulty(newDifficulty) == 1 )
	{
		luaL_error(L, "Bad format: enter difficulty as it appears in game - check documentation");
	}
	return 0;
}

#pragma endregion MISC

#pragma region PLAYER_OPTIONS

static int lua_GetAutoLevel(lua_State* L)
{
	int playOption = Memory::Read<int>(Misc::playOption);
	bool autoLevel = playOption >> 4 & 1;
	lua_pushboolean(L, autoLevel);
	return 1;
}

static int lua_SetAutoLevel(lua_State* L)
{
	int newAL = luaL_checkinteger(L, 1); // this is actually a bool
	int playOption = Memory::Read<int>(Misc::playOption);

	int mask = 1 << 4; // value is at the 5th bit 00010000
	playOption &= ~mask; // zero out the bit

	if (newAL == 1) { playOption |= mask; } // set it to 1 if true

	Memory::Write(Misc::playOption, playOption);

	return 0;
}

static int lua_GetTLI(lua_State* L)
{
	int playOption = Memory::Read<int>(Misc::playOption);
	bool TLI = playOption >> 5 & 1;
	lua_pushboolean(L, TLI);
	return 1;
}

static int lua_SetTLI(lua_State* L)
{
	int newTLI = luaL_checkinteger(L, 1);
	int playOption = Memory::Read<int>(Misc::playOption);

	int mask = 1 << 5; // value is at the 6th bit 00100000
	playOption &= ~mask;

	if (newTLI == 1) { playOption |= mask; }

	Memory::Write(Misc::playOption, playOption);

	return 0;
}

static int lua_GetReverseMouse(lua_State* L)
{
	int playOption = Memory::Read<int>(Misc::playOption);
	bool reverseMouse = playOption >> 6 & 1;
	lua_pushboolean(L, !reverseMouse); // inversed, see note below
	return 1;
}

static int lua_SetReverseMouse(lua_State* L)
{
	int newMouse = luaL_checkinteger(L, 1);
	int playOption = Memory::Read<int>(Misc::playOption);

	int mask = 1 << 6; // value is at the 7th bit
	playOption &= ~mask;

	// important note: this is inverted because the devs are insane and have
	// reversed mouse as the "default" in the code LOL
	if (newMouse == 0) { playOption |= mask; }

	Memory::Write(Misc::playOption, playOption);

	return 0;
}

#pragma endregion PLAYER_OPTIONS

#pragma region PATCHES

static int lua_EnableOrdnanceTweak(lua_State* L)
{
	float scalingFactor = static_cast<float>(luaL_checknumber(L, 1));
	EnableOrdnanceTweak(scalingFactor);
	return 0;
}

static int lua_UpdateOrdnance(lua_State* L)
{
	float vx = static_cast<float>(luaL_checknumber(L, 1));
	float vy = static_cast<float>(luaL_checknumber(L, 2));
	float vz = static_cast<float>(luaL_checknumber(L, 3));
	float px = static_cast<float>(luaL_checknumber(L, 4));
	float py = static_cast<float>(luaL_checknumber(L, 5));
	float pz = static_cast<float>(luaL_checknumber(L, 6));
	UpdateOrdnance(vx, vy, vz, px, py, pz);
	return 0;
}

static int lua_EnableShotConvergence(lua_State* L)
{
	EnableShotConvergence();
	return 0;
}

static int lua_SetSelectNone(lua_State* L)
{
	bool setting = luaL_checkinteger(L, 1); // this is being type checked in lua
	SetSelectNone(setting);
	return 0;
}

#pragma endregion PATCHES

#pragma region FUNCTION_HOOKS

static int lua_SetAsUser(lua_State* L) 
{
	void* handle = (void*)lua_touserdata(L, 1);
	GameObject* gameObject = GetObj(reinterpret_cast<unsigned int>(handle));
	FuncPtrs::SetAsUser(gameObject);
	return 0;
}

ControlPanel* p_ControlPanel = reinterpret_cast<ControlPanel*>(Misc::controlPanel);

static int lua_SelectOne(lua_State* L)
{
	void* handle = reinterpret_cast<void*>(lua_touserdata(L, 1));
	GameObject* gameObject = GetObj(reinterpret_cast<unsigned int>(handle));
	FuncPtrs::SelectOne(p_ControlPanel, gameObject);
	return 0;
}

static int lua_SelectNone(lua_State* L)
{
	FuncPtrs::SelectNone(p_ControlPanel);
	return 0;
}

static int lua_SelectAdd(lua_State* L)
{
	void* handle = reinterpret_cast<void*>(lua_touserdata(L, 1));
	GameObject* gameObject = GetObj(reinterpret_cast<unsigned int>(handle));
	FuncPtrs::SelectAdd(p_ControlPanel, gameObject);
	return 0;
}

#pragma endregion FUNCTION_HOOKS

#pragma region FILESYSTEM

static int lua_GetWorkingDirectory(lua_State* L)
{
	lua_pushstring(L, GetWorkingDirectory().c_str());
	SystemLog->Out(GetWorkingDirectory());
	return 1;
}

static int lua_MakeDirectory(lua_State* L)
{
	const char* directory = luaL_checkstring(L, 1);
	MakeDirectory(directory);
	return 0;
}

static int lua_FileRead(lua_State* L)
{
	const char* fileName = luaL_checkstring(L, 1);
	lua_pushstring(L, FileRead(fileName));
	return 1;
}

static int lua_FileWrite(lua_State* L)
{
	const char* fileName = luaL_checkstring(L, 1);
	const char* content = luaL_checkstring(L, 2);
	FileWrite(fileName, content);
	return 0;
}

#pragma endregion FILESYSTEM

#pragma region USER_LOGGING

// this feels very unsafe and it probably is LOL but it works
static int lua_LogOut(lua_State* L)
{
	auto instance = static_cast<Log*>(lua_touserdata(L, 1));
	const char* content = luaL_checkstring(L, 2);
	int level = luaL_optint(L, 3, 3);
	instance->Out(content, level);
	return 0;
}

static int lua_GetLogLevel(lua_State* L)
{
	auto instance = static_cast<Log*>(lua_touserdata(L, 1));
	int level = instance->GetLogLevel();
	lua_pushinteger(L, level);
	return 1;
}

static int lua_SetLogLevel(lua_State* L)
{
	auto instance = static_cast<Log*>(lua_touserdata(L, 1));
	int level = luaL_checkinteger(L, 2);
	instance->SetLogLevel(level);
	return 0;
}

static int lua_GetLogPath(lua_State* L)
{
	auto instance = static_cast<Log*>(lua_touserdata(L, 1));
	std::filesystem::path path = instance->GetLogPath();
	lua_pushstring(L, path.string().c_str());
	return 1;
}

static int lua_SetLogPath(lua_State* L)
{
	auto instance = static_cast<Log*>(lua_touserdata(L, 1));
	const char* path = luaL_checkstring(L, 2);
	instance->SetLogPath(path);
	return 0;
}

static int lua_CreateLog(lua_State* L)
{
	std::string path = luaL_checkstring(L, 1);
	int level = luaL_optint(L, 2, 3);

	auto instance = std::make_shared<Log>(Log(path, level));
	Log::userLogs.push_back(instance);

	lua_pushlightuserdata(L, instance.get());
	return 1;
}

#pragma endregion USER_LOGGING

#pragma region OGRE_HOOKS

// THESE NEED TO BE GLOBAL - if they are local they get misaligned on the
// stack when I make a new stack frame

float r{};
float g{};
float b{};
void* desiredLight{};

std::uint32_t SetDiffuseColour{};

static int lua_SetDiffuseColor(lua_State* L)
{
	std::string myLabel = luaL_checkstring(L, 1);
	r = static_cast<float>(luaL_checknumber(L, 2));
	g = static_cast<float>(luaL_checknumber(L, 3));
	b = static_cast<float>(luaL_checknumber(L, 4));

	for (const auto& unit : Hook::unitLights)
	{
		if (unit.label == myLabel)
		{
			desiredLight = *unit.light;
			break;
		}
	}

	if (desiredLight == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	SetDiffuseColour = static_cast<std::uint32_t>(setDiffuseColour);

	// wow this was utter BS to figure out
	__asm
	{
		// setLightDiffuse erroneously requires its args to be at an offset of
		// ebp+8 rather than ebp+4 so we subtract 4 from esp before the call to
		// fix this and align the stack properly
		push ebp // make new stack frame for good measure
		mov ebp, esp

		// I don't know which of these will fk stuff up so I'm just preserving all
		// the general purpose registers as well as xmm0
		push eax
		push ebx
		push ecx
		push edi
		push esi
		push edx

		sub esp, 0x10
		movdqu [esp], xmm0

		sub esp, 0x4 // align stack

		movss xmm0, [b]
		movss[esp], xmm0
		sub esp, 4

		movss xmm0, [g]
		movss[esp], xmm0
		sub esp, 4

		movss xmm0, [r]
		movss[esp], xmm0

		mov ecx, [desiredLight]
		call SetDiffuseColour

		movdqu xmm0, [esp]
		add esp, 0x10

		pop edx
		pop esi
		pop edi
		pop ecx
		pop ebx
		pop eax

		mov esp, ebp
		pop ebp
	}

	lua_pushboolean(L, true);
	return 1;
}

std::uint32_t SetSpecularColour{};

// this is essentially the same as diffuse color, gotta love OOP
static int lua_SetSpecularColor(lua_State* L)
{
	std::string myLabel = luaL_checkstring(L, 1);
	r = static_cast<float>(luaL_checknumber(L, 2));
	g = static_cast<float>(luaL_checknumber(L, 3));
	b = static_cast<float>(luaL_checknumber(L, 4));

	for (const auto& unit : Hook::unitLights)
	{
		if (unit.label == myLabel)
		{
			desiredLight = *unit.light;
			break;
		}
	}

	if (desiredLight == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	SetSpecularColour = static_cast<std::uint32_t>(setSpecularColour);

	__asm
	{
		push ebp
		mov ebp, esp

		push eax
		push ebx
		push ecx
		push edi
		push esi
		push edx

		sub esp, 0x10
		movdqu[esp], xmm0

		sub esp, 0x4 // align stack

		movss xmm0, [b]
		movss[esp], xmm0
		sub esp, 4

		movss xmm0, [g]
		movss[esp], xmm0
		sub esp, 4

		movss xmm0, [r]
		movss[esp], xmm0

		mov ecx, [desiredLight]
		call SetSpecularColour

		movdqu xmm0, [esp]
		add esp, 0x10

		pop edx
		pop esi
		pop edi
		pop ecx
		pop ebx
		pop eax

		mov esp, ebp
		pop ebp
	}

	lua_pushboolean(L, true);
	return 1;
}

float innerAngle{};
float outerAngle{};
float falloff{};

std::uint32_t SetSpotlightRange{};

static int lua_SetSpotlightRange(lua_State* L)
{
	std::string myLabel = luaL_checkstring(L, 1);
	innerAngle = static_cast<float>(luaL_checknumber(L, 2));
	outerAngle = static_cast<float>(luaL_checknumber(L, 3));
	falloff = static_cast<float>(luaL_checknumber(L, 4));

	for (const auto& unit : Hook::unitLights)
	{
		if (unit.label == myLabel)
		{
			desiredLight = *unit.light;
			break;
		}
	}

	if (desiredLight == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

	SetSpotlightRange = static_cast<std::uint32_t>(setSpotlightRange);

	__asm
	{
		push ebp
		mov ebp, esp

		push eax
		push ebx
		push ecx
		push edi
		push esi
		push edx

		sub esp, 0x10
		movdqu[esp], xmm0

		sub esp, 0x4 // align stack

		movss xmm0, [falloff]
		movss[esp], xmm0

		lea eax, [outerAngle]
		push eax

		lea eax, [innerAngle]
		push eax

		mov ecx, [desiredLight]
		call SetSpotlightRange

		movdqu xmm0, [esp]
		add esp, 0x10

		pop edx
		pop esi
		pop edi
		pop ecx
		pop ebx
		pop eax

		mov esp, ebp
		pop ebp
	}

	lua_pushboolean(L, true);
	return 1;
}

#pragma endregion OGRE_HOOKS

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

extern "C" 
{
	int __declspec(dllexport) luaopen_exu(lua_State* L)
	{

		const luaL_Reg exu_export[] = {
			// EXU Functions:
			{ "GetVersion",          lua_GetVersion          },
			{ "SetAccessMode",       lua_SetAccessMode       },
			{ "GetObj",			 	 lua_GetObj              },
			{ "GetGravity",			 lua_GetGravity          },
			{ "SetGravity",	         lua_SetGravity          },
			{ "GetReticleAngle",	 lua_GetReticleAngle     },
			{ "GetReticlePos",		 lua_GetReticlePos       },
			{ "GetSmartCursorRange", lua_GetSmartCursorRange },
			{ "SetSmartCursorRange", lua_SetSmartCursorRange },
			{ "GetSatState",         lua_GetSatState         },
			{ "GetSatCursorPos",     lua_GetSatCursorPos     },
			{ "GetSatCamPos",        lua_GetSatCamPos        },
			{ "GetSatClickPos",      lua_GetSatClickPos      },
			{ "GetSatPanSpeed",      lua_GetSatPanSpeed      },
			{ "SetSatPanSpeed",      lua_SetSatPanSpeed      },
			{ "GetMinSatZoom",       lua_GetMinSatZoom       },
			{ "SetMinSatZoom",       lua_SetMinSatZoom       },
			{ "GetMaxSatZoom",       lua_GetMaxSatZoom       },
			{ "SetMaxSatZoom",       lua_SetMaxSatZoom       },
			{ "GetSatZoom",          lua_GetSatZoom          },
			{ "SetSatZoom",          lua_SetSatZoom          },
			{ "GetRadarState",	     lua_GetRadarState       },
			{ "SetRadarState",		 lua_SetRadarState       },
			{ "GetZoomFactor",       lua_GetZoomFactor       },
			{ "SetZoomFactor",       lua_SetZoomFactor       },
			{ "GetMinZoomFactor",    lua_GetMinZoomFactor    },
			{ "SetMinZoomFactor",    lua_SetMinZoomFactor    },
			{ "GetMaxZoomFactor",    lua_GetMaxZoomFactor    },
			{ "SetMaxZoomFactor",    lua_SetMaxZoomFactor    },
			{ "GetGameKey",          lua_GetGameKey          },
			{ "GetSteam64",          lua_GetSteam64          },
			{ "GetWeaponMask",       lua_GetWeaponMask       },
			{ "GetLives",            lua_GetLives            },
			{ "SetLives",            lua_SetLives            },
			{ "EnableOrdnanceTweak", lua_EnableOrdnanceTweak },
			{ "UpdateOrdnance",      lua_UpdateOrdnance      },
			{ "EnableShotConvergence", lua_EnableShotConvergence },
			{ "SetSelectNone",       lua_SetSelectNone       },
			{ "GetDifficulty",       lua_GetDifficulty       },
			{ "SetDifficulty",       lua_SetDifficulty       },
			{ "GetAutoLevel",        lua_GetAutoLevel        },
			{ "SetAutoLevel",        lua_SetAutoLevel        },
			{ "GetTLI",              lua_GetTLI              },
			{ "SetTLI",              lua_SetTLI              },
			{ "GetReverseMouse",     lua_GetReverseMouse     },
			{ "SetReverseMouse",     lua_SetReverseMouse     },
			{ "SetAsUser",           lua_SetAsUser           },
			{ "SelectOne",           lua_SelectOne           },
			{ "SelectNone",          lua_SelectNone          },
			{ "SelectAdd",           lua_SelectAdd           },
			{ "GetWorkingDirectory", lua_GetWorkingDirectory },
			{ "MakeDirectory",       lua_MakeDirectory       },
			{ "FileRead",            lua_FileRead            },
			{ "FileWrite",           lua_FileWrite           },
			{ "LogOut",              lua_LogOut              },
			{ "CreateLog",           lua_CreateLog           },
			{ "SetDiffuseColor",     lua_SetDiffuseColor     },
			{ "SetSpecularColor",    lua_SetSpecularColor    },
			{ "SetSpotlightRange",   lua_SetSpotlightRange   },
			{0,                      0                       }
		};
		luaL_register(L, "exu", exu_export);
		return 0;
	}
}