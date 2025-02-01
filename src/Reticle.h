#pragma once

#include "BZR.h"
#include "Offset.h"

#include <lua.hpp>

#undef GetObject // windows.h name conflict

namespace ExtraUtilities::Lua::Reticle
{
	inline Offset position(BZR::Reticle::position);
	inline Offset range(BZR::Reticle::range);
	inline Offset object(BZR::Reticle::object);
	inline Offset matrix(BZR::Reticle::matrix);

	int GetPosition(lua_State* L);
	int GetRange(lua_State* L);
	int SetRange(lua_State* L);
	int GetObject(lua_State* L);
	int GetMatrix(lua_State* L);
}