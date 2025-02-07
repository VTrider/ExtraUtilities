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

#include "Radar.h"

namespace ExtraUtilities::Lua::Radar
{
	int GetState(lua_State* L)
	{
		lua_pushnumber(L, state.Read());
		return 1;
	}

	int SetState(lua_State* L)
	{
		uint8_t newState = static_cast<uint8_t>(luaL_checkinteger(L, 1));
		if (newState != 0 && newState != 1)
		{
			luaL_error(L, "Invalid input: options are: 0, 1");
		}
		state.Write(newState);
		return 0;
	}
}