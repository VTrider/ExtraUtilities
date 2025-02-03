/* Copyright (C) 2023-2025 VTrider
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

#pragma once

#include "BZR.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua
{
	inline void PushVector(lua_State* L, const BZR::VECTOR_3D& v)
	{
		lua_getglobal(L, "SetVector");

		lua_pushnumber(L, v.x);
		lua_pushnumber(L, v.y);
		lua_pushnumber(L, v.z);

		lua_call(L, 3, 1);
	}

	inline void PushMatrix(lua_State* L, const BZR::MAT_3D& m)
	{
		lua_getglobal(L, "SetMatrix");

		// These are in the "wrong" order because the game lua function is broken
		lua_pushnumber(L, m.up_x);
		lua_pushnumber(L, m.up_y);
		lua_pushnumber(L, m.up_z);
		lua_pushnumber(L, m.right_x);
		lua_pushnumber(L, m.right_y);
		lua_pushnumber(L, m.right_z);
		lua_pushnumber(L, m.front_x);
		lua_pushnumber(L, m.front_y);
		lua_pushnumber(L, m.front_z);
		lua_pushnumber(L, m.posit_x);
		lua_pushnumber(L, m.posit_y);
		lua_pushnumber(L, m.posit_z);

		lua_call(L, 12, 1);
	}

	inline bool CheckBool(lua_State* L, int idx)
	{
		if (!lua_isboolean(L, idx))
		{
			luaL_typerror(L, idx, "bool");
		}
		return lua_toboolean(L, idx);
	}

	inline void* CheckHandle(lua_State* L, int idx)
	{
		if (!lua_isuserdata(L, idx))
		{
			luaL_typerror(L, idx, "handle");
		}
		return lua_touserdata(L, idx);
	}
}