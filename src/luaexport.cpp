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
#include "Audio.h"
#include "bzfunc.h"
#include "exumeta.h"
#include "filesystem.h"
#include "Hook.h"
#include "Log.h"
#include "Memory.h"
#include "Offsets.h"

#include "lua.hpp"
#include <sol/sol.hpp>
#include <Windows.h>

#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#pragma region IMPORTANT_FUNCTIONS

std::unique_ptr<sol::state_view> lua;

static int exu_Init(lua_State* L)
{
	lua = std::make_unique<sol::state_view>(L);

	auto exu_api = (*lua)["exu_api"].get_or_create<sol::table>();

	// Metadata

	exu_api.set_function("GetVersion", []() { return Exu::version.c_str(); });

	// Important Misc

	exu_api.set_function("SetAccessMode", &Memory::SetAccessMode);
	exu_api.set_function("SysLogOut", [](const char* content, int level)
		{
			SystemLog->Out(content, level);
		});

	// Environment

	exu_api.set_function("GetGravity", []()
		{
			auto SetVector = lua->get<sol::function>("SetVector");
			VECTOR_3D gravity = Environment::GetGravity();
			return SetVector(gravity.x, gravity.y, gravity.z);
		});
	exu_api.set_function("SetGravity", &Environment::SetGravity);

	// Reticle

	exu_api.set_function("GetReticleAngle", &Reticle::GetReticleAngle);
	exu_api.set_function("GetReticlePos", []()
		{
			auto SetVector = lua->get<sol::function>("SetVector");
			VECTOR_3D reticlePos = Reticle::GetReticlePos();
			return SetVector(reticlePos.x, reticlePos.y, reticlePos.z);
		});
	exu_api.set_function("GetSmartCursorRange", &Reticle::GetSmartCursorRange);
	exu_api.set_function("SetSmartCursorRange", &Reticle::SetSmartCursorRange);
	exu_api.set_function("GetReticleObject", &Reticle::GetReticleObject);

	// Satellite

	exu_api.set_function("GetSatState", &Satellite::GetSatState);
	exu_api.set_function("GetSatCursorPos", []()
		{
			auto SetVector = lua->get<sol::function>("SetVector");
			VECTOR_3D cursorPos = Satellite::GetSatCursorPos();
			return SetVector(cursorPos.x, cursorPos.y, cursorPos.z);
		});
	exu_api.set_function("GetSatCamPos", []()
		{
			auto SetVector = lua->get<sol::function>("SetVector");
			VECTOR_3D camPos = Satellite::GetSatCamPos();
			return SetVector(camPos.x, camPos.y, camPos.z);
		});
	exu_api.set_function("GetSatClickPos", []()
		{
			auto SetVector = lua->get<sol::function>("SetVector");
			VECTOR_3D clickPos = Satellite::GetSatClickPos();
			return SetVector(clickPos.x, clickPos.y, clickPos.z);
		});
	exu_api.set_function("GetSatPanSpeed", &Satellite::GetSatPanSpeed);
	exu_api.set_function("SetSatPanSpeed", &Satellite::SetSatPanSpeed);
	exu_api.set_function("GetMinSatZoom", &Satellite::GetMinSatZoom);
	exu_api.set_function("SetMinSatZoom", &Satellite::SetMinSatZoom);
	exu_api.set_function("GetMaxSatZoom", &Satellite::GetMaxSatZoom);
	exu_api.set_function("SetMaxSatZoom", &Satellite::SetMaxSatZoom);
	exu_api.set_function("GetSatZoom", &Satellite::GetSatZoom);
	exu_api.set_function("SetSatZoom", &Satellite::SetSatZoom);

	// Radar

	exu_api.set_function("GetRadarState", &Radar::GetRadarState);
	exu_api.set_function("SetRadarState", &Radar::SetRadarState);

	// Camera

	exu_api.set_function("GetZoomFactor", &Camera::GetZoomFactor);
	exu_api.set_function("SetZoomFactor", &Camera::SetZoomFactor);
	exu_api.set_function("GetMinZoomFactor", &Camera::GetMinZoomFactor);
	exu_api.set_function("SetMinZoomFactor", &Camera::SetMinZoomFactor);
	exu_api.set_function("GetMaxZoomFactor", &Camera::GetMaxZoomFactor);
	exu_api.set_function("SetMaxZoomFactor", &Camera::SetMaxZoomFactor);

	// IO

	exu_api.set_function("GetGameKey", [](std::string key) 
		{
			int vKey = IO::GetKeyCode(key);
			return IO::GetGameKey(vKey);
		});

	// Misc

	exu_api.set_function("GetSteam64", &Misc::GetSteam64);
	exu_api.set_function("GetWeaponMask", &Misc::GetWeaponMask);
	exu_api.set_function("GetLives", &Misc::GetLives);
	exu_api.set_function("SetLives", &Misc::SetLives);
	exu_api.set_function("GetDifficulty", &Misc::GetDifficulty);
	exu_api.set_function("SetDifficulty", &Misc::SetDifficulty);

	// Play Options

	exu_api.set_function("GetAutoLevel", []() 
		{
			int playOption = Memory::Read<int>(Misc::playOption);
			bool autoLevel = playOption >> 4 & 1;
			return autoLevel;
		});
	exu_api.set_function("SetAutoLevel", [](bool newAL)
		{
			int playOption = Memory::Read<int>(Misc::playOption);

			int mask = 1 << 4; // value is at the 5th bit 00010000
			playOption &= ~mask; // zero out the bit

			if (newAL == 1) { playOption |= mask; } // set it to 1 if true

			Memory::Write(Misc::playOption, playOption);
		});
	exu_api.set_function("GetTLI", []()
		{
			int playOption = Memory::Read<int>(Misc::playOption);
			bool TLI = playOption >> 5 & 1;
			return TLI;
		});
	exu_api.set_function("SetTLI", [](bool newTLI)
		{
			int playOption = Memory::Read<int>(Misc::playOption);

			int mask = 1 << 5; // value is at the 5th bit 00010000
			playOption &= ~mask; // zero out the bit

			if (newTLI == 1) { playOption |= mask; } // set it to 1 if true

			Memory::Write(Misc::playOption, playOption);
		});
	exu_api.set_function("GetReverseMouse", []()
		{
			int playOption = Memory::Read<int>(Misc::playOption);
			bool reverseMouse = playOption >> 6 & 1;
			return reverseMouse;
		});
	exu_api.set_function("SetReverseMouse", [](bool newMouse)
		{
			int playOption = Memory::Read<int>(Misc::playOption);

			int mask = 1 << 6; // value is at the 7th bit
			playOption &= ~mask;

			// important note: this is inverted because the devs are insane and have
			// reversed mouse as the "default" in the code LOL
			if (newMouse == 0) { playOption |= mask; }

			Memory::Write(Misc::playOption, playOption);
		});

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

static int lua_EnableShotConvergence(lua_State*)
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

static int lua_SelectNone(lua_State*)
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
	int level = instance->GetLevel();
	lua_pushinteger(L, level);
	return 1;
}

static int lua_SetLogLevel(lua_State* L)
{
	auto instance = static_cast<Log*>(lua_touserdata(L, 1));
	int level = luaL_checkinteger(L, 2);
	instance->SetLevel(level);
	return 0;
}

static int lua_GetLogPath(lua_State* L)
{
	auto instance = static_cast<Log*>(lua_touserdata(L, 1));
	std::filesystem::path path = instance->GetPath();
	lua_pushstring(L, path.string().c_str());
	return 1;
}

static int lua_SetLogPath(lua_State* L)
{
	auto instance = static_cast<Log*>(lua_touserdata(L, 1));
	const char* path = luaL_checkstring(L, 2);
	instance->SetPath(path);
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

#pragma region AUDIO_SYSTEM

static int lua_PlaySound(lua_State* L)
{
	std::string soundPath = luaL_checkstring(L, 1);
	ALuint source = Audio::PlaySoundEffect(soundPath);
	lua_pushnumber(L, source);
	return 1;
}

static int lua_GetMainVolume(lua_State* L)
{
	float volume = Audio::GetMainVolume();
	lua_pushnumber(L, volume);
	return 1;
}

static int lua_SetMainVolume(lua_State* L)
{
	float volume = static_cast<float>(luaL_checknumber(L, 1));
	Audio::SetMainVolume(volume);
	return 0;
}

static int lua_GetVolume(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	float volume = Audio::GetVolume(source);
	lua_pushnumber(L, volume);
	return 1;
}

static int lua_SetVolume(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	float volume = static_cast<float>(luaL_checknumber(L, 2));
	Audio::SetVolume(source, volume);
	return 0;
}

static int lua_GetPaused(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	bool paused = Audio::GetPaused(source);
	lua_pushboolean(L, paused);
	return 1;
}

static int lua_SetPaused(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	bool paused = luaL_checkinteger(L, 2);
	Audio::SetPaused(source, paused);
	return 0;
}

static int lua_Stop(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	Audio::Stop(source);
	return 0;
}

static int lua_GetLooping(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	bool looping = Audio::GetLooping(source);
	lua_pushboolean(L, looping);
	return 1;
}

static int lua_SetLooping(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	bool looping = luaL_checkinteger(L, 2);
	Audio::SetLooping(source, looping);
	return 0;
}

static int lua_GetListenerTransform(lua_State* L)
{
	float posX = static_cast<float>(luaL_checknumber(L, 1));
	float posY = static_cast<float>(luaL_checknumber(L, 2));
	float posZ = static_cast<float>(luaL_checknumber(L, 3));

	float velX = static_cast<float>(luaL_checknumber(L, 4));
	float velY = static_cast<float>(luaL_checknumber(L, 5));
	float velZ = static_cast<float>(luaL_checknumber(L, 6));

	float frontX = static_cast<float>(luaL_checknumber(L, 7));
	float frontY = static_cast<float>(luaL_checknumber(L, 8));
	float frontZ = static_cast<float>(luaL_checknumber(L, 9));

	float upX = static_cast<float>(luaL_checknumber(L, 10));
	float upY = static_cast<float>(luaL_checknumber(L, 11));
	float upZ = static_cast<float>(luaL_checknumber(L, 12));

	// negate the Z values because OpenAL uses right handed coordinates
	// while BZ uses left handed
	float pos[] = { posX, posY, -posZ };
	float vel[] = { velX, velY, -velZ };
	float orientation[] = { frontX, frontY, -frontZ, upX, upY, -upZ };

	Audio::SetListenerTransform(pos, vel, orientation);

	return 0;
}

static int lua_GetSourceTransform(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);

	float posX = static_cast<float>(luaL_checknumber(L, 2));
	float posY = static_cast<float>(luaL_checknumber(L, 3));
	float posZ = static_cast<float>(luaL_checknumber(L, 4));

	float velX = static_cast<float>(luaL_checknumber(L, 5));
	float velY = static_cast<float>(luaL_checknumber(L, 6));
	float velZ = static_cast<float>(luaL_checknumber(L, 7));

	float pos[] = { posX, posY, -posZ };
	float vel[] = { velX, velY, -velZ };

	Audio::SetSourceTransform(source, pos, vel);

	return 0;
}

static int lua_GetSourceRefDist(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	float refDist = Audio::GetSourceRefDist(source);
	lua_pushnumber(L, refDist);
	return 1;
}

static int lua_SetSourceRefDist(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	float refDist = static_cast<float>(luaL_checknumber(L, 2));
	Audio::SetSourceRefDist(source, refDist);
	return 0;
}

static int lua_GetSourceRolloff(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	float rolloff = Audio::GetSourceRolloff(source);
	lua_pushnumber(L, rolloff);
	return 1;
}

static int lua_SetSourceRolloff(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	float rolloff = static_cast<float>(luaL_checknumber(L, 2));
	Audio::SetSourceRolloff(source, rolloff);
	return 0;
}

static int lua_GetSourceMaxDist(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	float maxDist = Audio::GetSourceMaxDist(source);
	lua_pushnumber(L, maxDist);
	return 1;
}

static int lua_SetSourceMaxDist(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	float maxDist = static_cast<float>(luaL_checknumber(L, 2));
	Audio::SetSourceMaxDist(source, maxDist);
	return 0;
}

static int lua_SourceIsMono(lua_State* L)
{
	ALuint source = luaL_checkinteger(L, 1);
	bool mono = Audio::IsMono(source);
	lua_pushboolean(L, mono);
	return 1;
}

#pragma endregion AUDIO_SYSTEM

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

extern "C" 
{
	int __declspec(dllexport) luaopen_exu(lua_State* L)
	{

		const luaL_Reg exu_export[] = {
			// EXU Functions:
			{ "Init",                exu_Init                },
			{ "GetObj",			 	 lua_GetObj              },
			{ "EnableOrdnanceTweak", lua_EnableOrdnanceTweak },
			{ "UpdateOrdnance",      lua_UpdateOrdnance      },
			{ "EnableShotConvergence", lua_EnableShotConvergence },
			{ "SetSelectNone",       lua_SetSelectNone       },
			{ "SetAsUser",           lua_SetAsUser           },
			{ "SelectOne",           lua_SelectOne           },
			{ "SelectNone",          lua_SelectNone          },
			{ "SelectAdd",           lua_SelectAdd           },
			{ "GetWorkingDirectory", lua_GetWorkingDirectory },
			{ "MakeDirectory",       lua_MakeDirectory       },
			{ "FileRead",            lua_FileRead            },
			{ "FileWrite",           lua_FileWrite           },
			{ "LogOut",              lua_LogOut              },
			{ "GetLogLevel",         lua_GetLogLevel         },
			{ "SetLogLevel",         lua_SetLogLevel         },
			{ "GetLogPath",          lua_GetLogPath          },
			{ "SetLogPath",          lua_SetLogPath          },
			{ "CreateLog",           lua_CreateLog           },
			{ "SetDiffuseColor",     lua_SetDiffuseColor     },
			{ "SetSpecularColor",    lua_SetSpecularColor    },
			{ "SetSpotlightRange",   lua_SetSpotlightRange   },
			{ "PlaySound",           lua_PlaySound           },
			{ "GetMainVolume",       lua_GetMainVolume       },
			{ "SetMainVolume",       lua_SetMainVolume       },
			{ "GetVolume",           lua_GetVolume           },
			{ "SetVolume",           lua_SetVolume           },
			{ "GetPaused",           lua_GetPaused           },
			{ "SetPaused",           lua_SetPaused           },
			{ "Stop",                lua_Stop                },
			{ "GetLooping",          lua_GetLooping          },
			{ "SetLooping",          lua_SetLooping          },
			{ "GetListenerTransform",lua_GetListenerTransform},
			{ "GetSourceTransform",  lua_GetSourceTransform  },
			{ "GetSourceRefDist",    lua_GetSourceRefDist    },
			{ "SetSourceRefDist",    lua_SetSourceRefDist    },
			{ "GetSourceRolloff",    lua_GetSourceRolloff    },
			{ "SetSourceRolloff",    lua_SetSourceRolloff    },
			{ "GetSourceMaxDist",    lua_GetSourceMaxDist    },
			{ "SetSourceMaxDist",    lua_SetSourceMaxDist    },
			{ "SourceIsMono",        lua_SourceIsMono        },
			{0,                      0                       }
		};
		luaL_register(L, "exu", exu_export);
		return 0;
	}
}