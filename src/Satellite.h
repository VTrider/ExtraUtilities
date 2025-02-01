#pragma once

#include "BZR.h"
#include "Offset.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Satellite
{
	int GetState(lua_State* L);
	int GetPosition(lua_State* L);
	int GetCameraPos(lua_State* L);
	int GetClickPos(lua_State* L);
	int GetPanSpeed(lua_State* L);
	int SetPanSpeed(lua_State* L);
	int GetMinZoom(lua_State* L);
	int SetMinZoom(lua_State* L);
	int GetMaxZoom(lua_State* L);
	int SetMaxZoom(lua_State* L);
	int GetZoom(lua_State* L);
	int SetZoom(lua_State* L);
}