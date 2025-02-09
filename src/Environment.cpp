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

#include "Environment.h"

#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::Environment
{
	int GetGravity(lua_State* L)
	{
		BZR::VECTOR_3D g = gravity.Read();

		PushVector(L, g);

		return 1;
	}

	// You can use individual floats or a vector 3d
	int SetGravity(lua_State* L)
	{
		BZR::VECTOR_3D newGravity;
		if (lua_gettop(L) > 1)
		{
			newGravity.x = static_cast<float>(static_cast<float>(luaL_checknumber(L, 1)));
			newGravity.y = static_cast<float>(luaL_checknumber(L, 2));
			newGravity.z = static_cast<float>(luaL_checknumber(L, 3));
		}
		else
		{
			int vectorIdx = lua_gettop(L);

			lua_getfield(L, vectorIdx, "x");
			newGravity.x = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, vectorIdx, "y");
			newGravity.y = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, vectorIdx, "z");
			newGravity.z = static_cast<float>(luaL_checknumber(L, -1));
		}
		gravity.Write(newGravity);

		return 0;
	}

	int GetFog(lua_State* L)
	{
		OgreFog fog = Patch::fog->Read();

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

		return 1;
	}

	int SetFog(lua_State* L)
	{
		float r = static_cast<float>(luaL_checknumber(L, 1));
		float g = static_cast<float>(luaL_checknumber(L, 2));
		float b = static_cast<float>(luaL_checknumber(L, 3));
		float start = static_cast<float>(luaL_checknumber(L, 4));
		float ending = static_cast<float>(luaL_checknumber(L, 5));

		Patch::fog->Write({ r, g, b, start, ending });

		return 0;
	}

	int GetSunAmbient(lua_State* L)
	{
		OgreColor sun = Patch::sunAmbient->Read();

		lua_createtable(L, 0, 3);

		lua_pushnumber(L, sun.r);
		lua_setfield(L, -2, "r");

		lua_pushnumber(L, sun.g);
		lua_setfield(L, -2, "g");

		lua_pushnumber(L, sun.b);
		lua_setfield(L, -2, "b");

		return 1;
	}

	int SetSunAmbient(lua_State* L)
	{
		float r = static_cast<float>(luaL_checknumber(L, 1));
		float g = static_cast<float>(luaL_checknumber(L, 2));
		float b = static_cast<float>(luaL_checknumber(L, 3));

		Patch::sunAmbient->Write({r, g, b});

		return 0;
	}
}