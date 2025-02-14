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

#include "Reticle.h"

#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::Reticle
{
	int GetPosition(lua_State* L)
	{
		BZR::VECTOR_3D pos = position.Read();

		PushVector(L, pos);

		return 1;
	}

	int GetRange(lua_State* L)
	{
		lua_pushnumber(L, range.Read());
		return 1;
	}

	int SetRange(lua_State* L)
	{
		float newRange = static_cast<float>(luaL_checknumber(L, 1));
		range.Write(newRange);
		return 0;
	}

	int GetObject(lua_State* L)
	{
		BZR::handle h = object.Read();

		if (h == 0)
		{
			lua_pushnil(L);
			return 1;
		}
		else
		{
			lua_pushlightuserdata(L, reinterpret_cast<void*>(h));
		}
		
		return 1;
	}

	int GetMatrix(lua_State* L)
	{
		BZR::MAT_3D reticleMat = matrix.Read();

		PushMatrix(L, reticleMat);

		return 1;
	}
}