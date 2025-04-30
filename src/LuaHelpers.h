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
#include "Ogre.h"

#include <lua.hpp>

namespace ExtraUtilities
{
	// RAII stack cleanup tool, define at the beginning of a lua callback
	// so your local variables get cleaned up to prevent crashing.
	class StackGuard
	{
	private:
		lua_State* L;
		int top;

	public:
		StackGuard(lua_State* L) : L(L), top(lua_gettop(L)) {}

		~StackGuard()
		{
			lua_settop(L, top);
		}
	};
}

namespace ExtraUtilities::Lua
{
	// Pushes one bz vector to the stack
	inline void PushVector(lua_State* L, const BZR::VECTOR_3D& v)
	{
		lua_getglobal(L, "SetVector");
		
		lua_pushnumber(L, v.x);
		lua_pushnumber(L, v.y);
		lua_pushnumber(L, v.z);

		lua_call(L, 3, 1);
	}

	// Pushes one bz matrix to the stack
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

	// Pushes an ogre color table to the stack
	inline void PushColor(lua_State* L, const Ogre::Color& color)
	{
		lua_createtable(L, 0, 3);

		lua_pushnumber(L, color.r);
		lua_setfield(L, -2, "r");

		lua_pushnumber(L, color.g);
		lua_setfield(L, -2, "g");

		lua_pushnumber(L, color.b);
		lua_setfield(L, -2, "b");
	}

	// Pushes an ogre fog table to the stack
	inline void PushFog(lua_State* L, const Ogre::Fog& fog)
	{
		lua_createtable(L, 0, 5);

		lua_pushnumber(L, fog.r);
		lua_setfield(L, -2, "r");

		lua_pushnumber(L, fog.g);
		lua_setfield(L, -2, "g");

		lua_pushnumber(L, fog.b);
		lua_setfield(L, -2, "b");

		lua_pushnumber(L, fog.start);
		lua_setfield(L, -2, "start");

		lua_pushnumber(L, fog.ending);
		lua_setfield(L, -2, "ending");
	}

	// Type checked lua boolean
	inline bool CheckBool(lua_State* L, int idx)
	{
		if (!lua_isboolean(L, idx))
		{
			luaL_typerror(L, idx, "bool");
		}
		return lua_toboolean(L, idx);
	}

	// Type checked lua handle
	inline BZR::handle CheckHandle(lua_State* L, int idx)
	{
		if (!lua_isuserdata(L, idx))
		{
			luaL_typerror(L, idx, "handle");
		}
		return reinterpret_cast<BZR::handle>(lua_touserdata(L, idx));
	}

	// Get either a vector or three numbers from lua
	inline BZR::VECTOR_3D CheckVectorOrSingles(lua_State* L, int idx)
	{
		BZR::VECTOR_3D v;
		if (lua_isuserdata(L, idx))
		{
			lua_getfield(L, idx, "x");
			v.x = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, idx, "y");
			v.y = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, idx, "z");
			v.z = static_cast<float>(luaL_checknumber(L, -1));
		}
		else
		{
			v.x = static_cast<float>(luaL_checknumber(L, idx));
			v.y = static_cast<float>(luaL_checknumber(L, idx + 1));
			v.z = static_cast<float>(luaL_checknumber(L, idx + 2));
		}
		return v;
	}

	// Get either a color table or three numbers from lua
	inline Ogre::Color CheckColorOrSingles(lua_State* L, int idx)
	{
		Ogre::Color c;
		if (lua_istable(L, idx))
		{
			lua_getfield(L, idx, "r");
			c.r = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, idx, "g");
			c.g = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, idx, "b");
			c.b = static_cast<float>(luaL_checknumber(L, -1));
		}
		else
		{
			c.r = static_cast<float>(luaL_checknumber(L, idx));
			c.g = static_cast<float>(luaL_checknumber(L, idx + 1));
			c.b = static_cast<float>(luaL_checknumber(L, idx + 2));
		}
		return c;
	}

	// Gets either a fog table or five numbers from lua
	inline Ogre::Fog CheckFogOrSingles(lua_State* L, int idx)
	{
		Ogre::Fog f;
		if (lua_istable(L, idx))
		{
			lua_getfield(L, idx, "r");
			f.r = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, idx, "g");
			f.g = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, idx, "b");
			f.b = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, idx, "start");
			f.start = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, idx, "ending");
			f.ending = static_cast<float>(luaL_checknumber(L, -1));
		}
		else
		{
			f.r = static_cast<float>(luaL_checknumber(L, idx));
			f.g = static_cast<float>(luaL_checknumber(L, idx + 1));
			f.b = static_cast<float>(luaL_checknumber(L, idx + 2));
			f.start = static_cast<float>(luaL_checknumber(L, idx + 3));
			f.ending = static_cast<float>(luaL_checknumber(L, idx + 4));
		}
		return f;
	}
}