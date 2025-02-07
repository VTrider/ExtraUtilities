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
}