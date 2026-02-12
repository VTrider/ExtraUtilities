/* Copyright (C) 2023-2026 VTrider
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

#include "IO.h"

namespace ExtraUtilities::Lua::IO
{
	int GetGameKey(lua_State* L)
	{
		std::string key = luaL_checkstring(L, 1);
		
		auto it = keyMap.find(ToUpper(key));

		int vKey;

		if (it != keyMap.end())
		{
			vKey = it->second;
		}
		else
		{
			luaL_argerror(L, 1, "Extra Utilities Error: invalid key - see wiki");
			return 0;
		}

		// this ternary is necessary cause you need to evaluate the
		// truthiness in C++ due to the weird return value of GetAsyncKeyState
		lua_pushboolean(L, GetAsyncKeyState(vKey) ? true : false);

		return 1;
	}
}