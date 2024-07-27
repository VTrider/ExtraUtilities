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

#include "asm.h"
#include "bzfunc.h"
#include "filesystem.h"
#include "Hook.h"
#include "Memory.h"
#include "Offsets.h"

#include "lua.hpp"
#include <Windows.h>

#include <chrono>
#include <thread>
#include "Log.h"

extern Log* SystemLog;

/*---------------------
* Important Functions *
----------------------*/

int lua_GetVersion(lua_State* L) 
{
	lua_pushstring(L, "test");
	return 1;
}

int lua_SetAccessMode(lua_State* L)
{
	int mode = luaL_checkinteger(L, 1);
	Memory::SetAccessMode(mode);
	return 0;
}

int lua_GetObj(lua_State* L)
{
	void* handle = (void*)lua_touserdata(L, 1);
	GameObject* gameObject = GetObj((unsigned int)handle);
	lua_pop(L, 1);
	lua_pushlightuserdata(L, gameObject);
	return 1;
}

/*-----------------------
* Environment Functions *
----------------------- */

int lua_GetGravity(lua_State* L)
{
	VECTOR_3D gravity = Environment::GetGravity();
	lua_pop(L, 1);
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
	lua_pop(L, 3);
	Environment::SetGravity(x, y, z);
	return 0;
}

/*-------------------
* Reticle Functions *
------------------- */

int lua_GetReticleAngle(lua_State* L)
{
	lua_pushnumber(L, Reticle::GetReticleAngle());
	return 1;
}

int lua_GetReticlePos(lua_State* L) 
{
	try
	{
		VECTOR_3D reticlePos = Reticle::GetReticlePos();
		lua_pop(L, 1);
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

int lua_GetSmartCursorRange(lua_State* L)
{
	lua_pushnumber(L, Reticle::GetSmartCursorRange());
	return 1;
}

int lua_SetSmartCursorRange(lua_State* L)
{
	try
	{
		float range = static_cast<float>(luaL_checknumber(L, 1));
		lua_pop(L, 1);
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

/*---------------------
* Satellite Functions *
--------------------- */

int lua_GetSatState(lua_State* L)
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

int lua_GetSatCursorPos(lua_State* L)
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

int lua_GetSatCamPos(lua_State* L)
{
	VECTOR_3D camPos = Satellite::GetSatCamPos();
	lua_pop(L, 1);
	lua_createtable(L, 0, 3);

	lua_pushnumber(L, camPos.x);
	lua_setfield(L, -2, "x");

	lua_pushnumber(L, camPos.y);
	lua_setfield(L, -2, "y");

	lua_pushnumber(L, camPos.z);
	lua_setfield(L, -2, "z");

	return 1;
}

int lua_GetSatClickPos(lua_State* L)
{
	VECTOR_3D clickPos = Satellite::GetSatClickPos();
	lua_pop(L, 1);
	lua_createtable(L, 0, 3);

	lua_pushnumber(L, clickPos.x);
	lua_setfield(L, -2, "x");

	lua_pushnumber(L, clickPos.y);
	lua_setfield(L, -2, "y");

	lua_pushnumber(L, clickPos.z);
	lua_setfield(L, -2, "z");

	return 1;
}

int lua_GetSatPanSpeed(lua_State* L)
{
	lua_pushnumber(L, Satellite::GetSatPanSpeed());
	return 1;
}

int lua_SetSatPanSpeed(lua_State* L)
{
	try
	{
		float speed = static_cast<float>(luaL_checknumber(L, 1));
		lua_pop(L, 1);
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

int lua_GetMinSatZoom(lua_State* L)
{
	lua_pushnumber(L, Satellite::GetMinSatZoom());
	return 1;
}

int lua_SetMinSatZoom(lua_State* L)
{
	float zoom = static_cast<float>(luaL_checknumber(L, 1));
	lua_pop(L, 1);
	Satellite::SetMinSatZoom(zoom);
	return 0;
}

int lua_GetMaxSatZoom(lua_State* L)
{
	lua_pushnumber(L, Satellite::GetMaxSatZoom());
	return 1;
}

int lua_SetMaxSatZoom(lua_State* L)
{
	float zoom = static_cast<float>(luaL_checknumber(L, 1));
	lua_pop(L, 1);
	Satellite::SetMaxSatZoom(zoom);
	return 0;
}

int lua_GetSatZoom(lua_State* L)
{
	lua_pushnumber(L, Satellite::GetSatZoom());
	return 1;
}

int lua_SetSatZoom(lua_State* L)
{
	try
	{
		float zoom = static_cast<float>(luaL_checknumber(L, 1));
		lua_pop(L, 1);
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

/*-----------------
* Radar Functions *
------------------*/

int lua_GetRadarState(lua_State* L) 
{
	lua_pushnumber(L, Radar::GetRadarState());
	return 1;
}

int lua_SetRadarState(lua_State* L) 
{
	int state = luaL_checkinteger(L, 1);
	lua_pop(L, 1);
	Radar::SetRadarState(state);
	return 0;
}

/*------------------
* Camera Functions *
-------------------*/

int lua_GetZoomFactor(lua_State* L)
{
	std::string camera = luaL_checkstring(L, 1);
	lua_pushnumber(L, Camera::GetZoomFactor(camera));
	return 1;
}

int lua_SetZoomFactor(lua_State* L)
{
	float factor = static_cast<float>(luaL_checknumber(L, 1));
	std::string camera = luaL_checkstring(L, 2);
	Camera::SetZoomFactor(factor, camera);
	return 0;
}

int lua_GetMinZoomFactor(lua_State* L)
{
	lua_pushnumber(L, Camera::GetMinZoomFactor());
	return 1;
}

int lua_SetMinZoomFactor(lua_State* L)
{
	float factor = static_cast<float>(luaL_checknumber(L, 1));
	Camera::SetMinZoomFactor(factor);
	return 0;
}

int lua_GetMaxZoomFactor(lua_State* L)
{
	lua_pushnumber(L, Camera::GetMaxZoomFactor());
	return 1;
}

int lua_SetMaxZoomFactor(lua_State* L)
{
	float factor = static_cast<float>(luaL_checknumber(L, 1));
	Camera::SetMaxZoomFactor(factor);
	return 0;
}

/*--------------
* Input/Output *
---------------*/

int lua_GetGameKey(lua_State* L)
{
	std::string key = luaL_checkstring(L, 1);
	int vKey = (IO::GetKeyCode(key));
	lua_pushboolean(L, IO::GetGameKey(vKey));
	return 1;
}

/*----------------
* Misc Functions *
-----------------*/

int lua_GetSteam64(lua_State* L) 
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

int lua_GetWeaponMask(lua_State* L)
{
	lua_pushnumber(L, Misc::GetWeaponMask());
	
	return 1;
}

int lua_GetLives(lua_State* L)
{
	lua_pushnumber(L, Misc::GetLives());
	return 1;
}
// 
int lua_SetLives(lua_State* L)
{
	int newLives = luaL_checkinteger(L, 1);
	Misc::SetLives(newLives);
	return 1;
}

int lua_GetDifficulty(lua_State* L)
{
	lua_pushstring(L, Misc::GetDifficulty());
	return 1;
}

int lua_SetDifficulty(lua_State* L)
{
	std::string newDifficulty = luaL_checkstring(L, 1);
	if (Misc::SetDifficulty(newDifficulty) == 1 )
	{
		luaL_error(L, "Bad format: enter difficulty as it appears in game - check documentation");
	}
	return 0;
}

int lua_EnableOrdnanceTweak(lua_State* L)
{
	float scalingFactor = static_cast<float>(luaL_checknumber(L, 1));
	EnableOrdnanceTweak(scalingFactor);
	return 0;
}

int lua_UpdateOrdnance(lua_State* L)
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

int lua_EnableShotConvergence(lua_State* L)
{
	EnableShotConvergence();
	return 0;
}

/*-------------------------
* Internal Function Hooks *
--------------------------*/

int lua_SetAsUser(lua_State* L) 
{
	void* handle = (void*)lua_touserdata(L, 1);
	GameObject* gameObject = GetObj((unsigned int)handle);
	FuncPtrs::SetAsUser(gameObject);
	return 0;
}

/*------------
* Filesystem *
-------------*/

int lua_MakeDirectory(lua_State* L)
{
	const char* directory = luaL_checkstring(L, 1);
	MakeDirectory(directory);
	return 0;
}

int lua_FileRead(lua_State* L)
{
	const char* fileName = luaL_checkstring(L, 1);
	lua_pushstring(L, FileRead(fileName));
	return 1;
}

int lua_FileWrite(lua_State* L)
{
	const char* fileName = luaL_checkstring(L, 1);
	const char* content = luaL_checkstring(L, 2);
	FileWrite(fileName, content);
	return 0;
}

/*--------------
* User Logging *
---------------*/

int lua_LogGC(lua_State* L) {
	Log* instance = static_cast<Log*>(luaL_checkudata(L, 1, "LogMetatable"));
	instance->~Log();  // Explicitly call the destructor
	return 0;
}

int lua_LogOut(lua_State* L)
{
	Log* instance = static_cast<Log*>(luaL_checkudata(L, 1, "LogMetatable"));
	const char* content = luaL_checkstring(L, 2);
	int level = luaL_optinteger(L, 3, 3);
	instance->Out(content, level);
	return 0;
}

int lua_CreateLog(lua_State* L)
{
	std::string path = luaL_checkstring(L, 1);
	int level = luaL_optinteger(L, 2, 3);

	Log* instance = static_cast<Log*>(lua_newuserdata(L, sizeof(Log)));
	new (instance) Log(path, level); // placement new since lua already allocated the memory

	//luaL_newmetatable(L, "LogMetatable");

	//lua_pushstring(L, "__gc");
	//lua_pushcfunction(L, lua_LogGC);
	//lua_settable(L, -3);

	//lua_pushstring(L, "__index");
	//lua_newtable(L);

	//lua_pushstring(L, "Out");
	//lua_pushcfunction(L, lua_LogOut);
	//lua_settable(L, -3);

	//lua_settable(L, -3); // set methods table to __index

	//lua_setmetatable(L, -2); // set metatable for the log userdata

	return 1;
}

std::uint32_t SetDiffuseColor = static_cast<std::uint32_t>(Hooks::setDiffuseColor);

// THESE NEED TO BE GLOBAL - if they are local they get misaligned on the
// stack when I make a new stack frame
float r{};
float g{};
float b{};
void* desiredLight{};

int lua_SetDiffuseColor(lua_State* L)
{
	std::string myLabel = luaL_checkstring(L, 1);
	r = static_cast<float>(luaL_checknumber(L, 2));
	g = static_cast<float>(luaL_checknumber(L, 3));
	b = static_cast<float>(luaL_checknumber(L, 4));

	for (const auto& unit : Hook::unitLights)
	{
		if (unit.label == myLabel)
		{
			desiredLight = unit.light;
		}
	}

	if (desiredLight == nullptr)
	{
		lua_pushboolean(L, false);
		return 1;
	}

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
		call SetDiffuseColor

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

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

extern "C" 
{
	int __declspec(dllexport) luaopen_exu(lua_State* L)
	{

		const luaL_Reg exu_export[] = {
			// EXU Functions:
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
			{ "GetDifficulty",       lua_GetDifficulty       },
			{ "SetDifficulty",       lua_SetDifficulty       },
			{ "SetAsUser",           lua_SetAsUser           },
			{ "MakeDirectory",       lua_MakeDirectory       },
			{ "FileRead",            lua_FileRead            },
			{ "FileWrite",           lua_FileWrite           },
			{ "CreateLog",           lua_CreateLog           },
			{ "Test",                lua_Test                },
			{0,                      0                       }
		};
		luaL_register(L, "exu", exu_export);
		return 0;
	}
}