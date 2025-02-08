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

#include "Ordnance.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Ordnance
{
	int GetCoeffBallistic(lua_State* L)
	{
		lua_pushnumber(L, coeffBallistic.Read());
		return 1;
	}

	int SetCoeffBallistic(lua_State* L)
	{
		float newCoeff = static_cast<float>(luaL_checknumber(L, 1));
		coeffBallistic.Write(newCoeff);
		return 0;
	}
}