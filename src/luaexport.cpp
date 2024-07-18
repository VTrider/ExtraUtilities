/*----------------------------------
* Exported lua functions go here,  *
* sends/receieves data to/from the *
* lua stack						   *
-----------------------------------*/

#include "bzfunc.h"
#include "filesystem.h"
#include "lua.hpp"
#include <Windows.h>
#include <thread>
#include <chrono>
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
	VECTOR_3D gravity = GetGravity();
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
	float x = luaL_checknumber(L, 1);
	float y = luaL_checknumber(L, 2);
	float z = luaL_checknumber(L, 3);
	lua_pop(L, 3);
	SetGravity(x, y, z);
	return 0;
}

//int lua_GetLuminance(lua_State* L)
//{
//	Luminance* sunlight = GetLuminance();
//	lua_pop(L, 1);
//	lua_createtable(L, 0, 3);
//
//	lua_pushnumber(L, sunlight->r);
//	lua_setfield(L, -2, "r");
//
//	lua_pushnumber(L, sunlight->g);
//	lua_setfield(L, -2, "g");
//
//	lua_pushnumber(L, sunlight->b);
//	lua_setfield(L, -2, "b");
//
//	return 1;
//}
//
//int lua_SetLuminance(lua_State* L)
//{
//	float r = luaL_checknumber(L, 1);
//	float g = luaL_checknumber(L, 2);
//	float b = luaL_checknumber(L, 3);
//	lua_pop(L, 3);
//	SetLuminance(r, g, b);
//	return 0;
//}

//int lua_GetFogStart(lua_State* L)
//{
//	lua_pushnumber(L, GetFogStart());
//	return 1;
//}
//
//int lua_SetFogStart(lua_State* L)
//{
//	float value = luaL_checknumber(L, 1);
//	lua_pop(L, 1);
//	SetFogStart(value);
//	return 0;
//}

/*-------------------
* Reticle Functions *
------------------- */

int lua_GetReticleAngle(lua_State* L)
{
	lua_pushnumber(L, GetReticleAngle());
	return 1;
}

int lua_GetReticlePos(lua_State* L) 
{
	try
	{
		VECTOR_3D reticlePos = GetReticlePos();
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
	lua_pushnumber(L, GetSmartCursorRange());
	return 1;
}

int lua_SetSmartCursorRange(lua_State* L)
{
	try
	{
		float range = luaL_checknumber(L, 1);
		lua_pop(L, 1);
		SetSmartCursorRange(range);
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
		lua_pushnumber(L, GetSatState());
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
		VECTOR_3D* cursorPos = GetSatCursorPos();
		lua_pop(L, 1);
		lua_createtable(L, 0, 3);

		lua_pushnumber(L, cursorPos->x);
		lua_setfield(L, -2, "x");

		lua_pushnumber(L, cursorPos->y);
		lua_setfield(L, -2, "y");

		lua_pushnumber(L, cursorPos->z);
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
	VECTOR_3D* camPos = GetSatCamPos();
	lua_pop(L, 1);
	lua_createtable(L, 0, 3);

	lua_pushnumber(L, camPos->x);
	lua_setfield(L, -2, "x");

	lua_pushnumber(L, camPos->y);
	lua_setfield(L, -2, "y");

	lua_pushnumber(L, camPos->z);
	lua_setfield(L, -2, "z");

	return 1;
}

int lua_GetSatClickPos(lua_State* L)
{
	VECTOR_3D* clickPos = GetSatClickPos();
	lua_pop(L, 1);
	lua_createtable(L, 0, 3);

	lua_pushnumber(L, clickPos->x);
	lua_setfield(L, -2, "x");

	lua_pushnumber(L, clickPos->y);
	lua_setfield(L, -2, "y");

	lua_pushnumber(L, clickPos->z);
	lua_setfield(L, -2, "z");

	return 1;
}

int lua_GetSatPanSpeed(lua_State* L)
{
	lua_pushnumber(L, GetSatPanSpeed());
	return 1;
}

int lua_SetSatPanSpeed(lua_State* L)
{
	try
	{
		float speed = luaL_checknumber(L, 1);
		lua_pop(L, 1);
		SetSatPanSpeed(speed);
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
	lua_pushnumber(L, GetMinSatZoom());
	return 1;
}

int lua_SetMinSatZoom(lua_State* L)
{
	float zoom = luaL_checknumber(L, 1);
	lua_pop(L, 1);
	SetMinSatZoom(zoom);
	return 0;
}

int lua_GetMaxSatZoom(lua_State* L)
{
	lua_pushnumber(L, GetMaxSatZoom());
	return 1;
}

int lua_SetMaxSatZoom(lua_State* L)
{
	float zoom = luaL_checknumber(L, 1);
	lua_pop(L, 1);
	SetMaxSatZoom(zoom);
	return 0;
}

int lua_GetSatZoom(lua_State* L)
{
	lua_pushnumber(L, GetSatZoom());
	return 1;
}

int lua_SetSatZoom(lua_State* L)
{
	try
	{
		float zoom = luaL_checknumber(L, 1);
		lua_pop(L, 1);
		SetSatZoom(zoom);
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
----------------- */

int lua_GetRadarState(lua_State* L) 
{
	lua_pushnumber(L, GetRadarState());
	return 1;
}

int lua_SetRadarState(lua_State* L) 
{
	int state = luaL_checkinteger(L, 1);
	lua_pop(L, 1);
	SetRadarState(state);
	return 0;
}

/*--------------
* Input/Output *
---------------*/

int lua_GetGameKey(lua_State* L)
{
	std::string key = luaL_checkstring(L, 1);
	int vKey = (GetKeyCode(key));
	lua_pushboolean(L, GetGameKey(vKey));
	return 1;
}

/*----------------
* Misc Functions *
-----------------*/

int lua_GetSteam64(lua_State* L) 
{
	try
	{
		lua_pushstring(L, GetSteam64());
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
	lua_pushnumber(L, GetWeaponMask());
	
	return 1;
}

int lua_GetLives(lua_State* L)
{
	lua_pushnumber(L, GetLives());
	return 1;
}

int lua_SetLives(lua_State* L)
{
	int newLives = luaL_checkinteger(L, 1);
	lua_pop(L, 1);
	SetLives(newLives);
	return 1;
}

int lua_GetDifficulty(lua_State* L)
{
	lua_pushstring(L, GetDifficulty());
	return 1;
}

int lua_SetDifficulty(lua_State* L)
{
	std::string newDifficulty = luaL_checkstring(L, 1);
	if (SetDifficulty(newDifficulty) == 1 )
	{
		luaL_error(L, "Bad format: enter difficulty as it appears in game - check documentation");
	}
	return 0;
}

int lua_EnableOrdnanceTweak(lua_State* L)
{
	float scalingFactor = luaL_checknumber(L, 1);
	EnableOrdnanceTweak(scalingFactor);
	return 0;
}

int lua_UpdateOrdnance(lua_State* L)
{
	float vx = luaL_checknumber(L, 1);
	float vy = luaL_checknumber(L, 2);
	float vz = luaL_checknumber(L, 3);
	float px = luaL_checknumber(L, 4);
	float py = luaL_checknumber(L, 5);
	float pz = luaL_checknumber(L, 6);
	UpdateOrdnance(vx, vy, vz, px, py, pz);
	return 0;
}

/*-------------------------
* Internal Function Hooks *
--------------------------*/

int lua_SetAsUser(lua_State* L) 
{
	void* handle = (void*)lua_touserdata(L, 1);
	GameObject* gameObject = GetObj((unsigned int)handle);
	SetAsUser(gameObject);

	lua_pop(L, 1);
	return 0;
}

/*------------
* Filesystem *
-------------*/

int lua_MakeDirectory(lua_State* L)
{
	const char* directory = luaL_checkstring(L, 1);
	lua_pop(L, 1);
	MakeDirectory(directory);
	return 0;
}

int lua_FileRead(lua_State* L)
{
	const char* fileName = luaL_checkstring(L, 1);
	lua_pop(L, 1);
	lua_pushstring(L, FileRead(fileName));
	return 1;
}

int lua_FileWrite(lua_State* L)
{
	const char* fileName = luaL_checkstring(L, 1);
	const char* content = luaL_checkstring(L, 2);
	lua_pop(L, 2);
	FileWrite(fileName, content);
	return 0;
}

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

extern "C" 
{
	int __declspec(dllexport) luaopen_exu(lua_State* L)
	{

		const luaL_Reg exu_export[] = {
			// EXU Functions:
			{ "GetObj",			 	 lua_GetObj              },
			{ "GetGravity",			 lua_GetGravity          },
			{ "SetGravity",	         lua_SetGravity          },
			//{ "SetLuminance",        lua_SetLuminance        },
			//{ "GetLuminance",        lua_GetLuminance        },
			//{ "GetFogStart",         lua_GetFogStart         },
			//{ "SetFogStart",         lua_SetFogStart         },
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
			{ "GetGameKey",          lua_GetGameKey          },
			{ "GetSteam64",          lua_GetSteam64          },
			{ "GetWeaponMask",       lua_GetWeaponMask       },
			{ "GetLives",            lua_GetLives            },
			{ "SetLives",            lua_SetLives            },
			{ "EnableOrdnanceTweak", lua_EnableOrdnanceTweak },
			{ "UpdateOrdnance",      lua_UpdateOrdnance      },
			{ "GetDifficulty",       lua_GetDifficulty       },
			{ "SetDifficulty",       lua_SetDifficulty       },
			{ "SetAsUser",           lua_SetAsUser           },
			{ "MakeDirectory",       lua_MakeDirectory       },
			{ "FileRead",            lua_FileRead            },
			{ "FileWrite",           lua_FileWrite           },
			{0,                      0                       }
		};
		luaL_register(L, "exu", exu_export);
		return 0;
	}
}