#pragma once

#include "BZR.h"
#include "Offset.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Environment
{
	inline Offset gravity(BZR::Environment::gravityVector);

	int GetGravity(lua_State* L);
	int SetGravity(lua_State* L);
}