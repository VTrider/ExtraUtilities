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

#include "PlayOption.h"

#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::PlayOption
{
	int GetAutoLevel(lua_State* L)
	{
		bool levelBit = (playOption.Read() >> 4) & 1;
		lua_pushboolean(L, levelBit);
		return 1;
	}

	int SetAutoLevel(lua_State* L)
	{
		bool enabled = CheckBool(L, 1);

		uint8_t* p_playOption = playOption.Get();
		*p_playOption &= ~(1 << 4); // clear bit
		if (enabled)
		{
			*p_playOption |= (1 << 4); // set bit
		}
		return 0;
	}

	int GetTLI(lua_State* L)
	{
		bool TLIbit = (playOption.Read() >> 5) & 1;
		lua_pushboolean(L, TLIbit);
		return 1;
	}

	int SetTLI(lua_State* L)
	{
		bool enabled = CheckBool(L, 1);

		uint8_t* p_playOption = playOption.Get();
		*p_playOption &= ~(1 << 5); // clear bit
		if (enabled)
		{
			*p_playOption |= (1 << 5); // set bit
		}
		return 0;
	}

	int GetReverseMouse(lua_State* L)
	{
		bool reverseMouseBit = (playOption.Read() >> 6) & 1;
		lua_pushboolean(L, reverseMouseBit);
		return 1;
	}

	int SetReverseMouse(lua_State* L)
	{
		bool enabled = CheckBool(L, 1);

		uint8_t* p_playOption = playOption.Get();
		*p_playOption &= ~(1 << 6); // clear bit
		if (enabled)
		{
			*p_playOption |= (1 << 6); // set bit
		}
		return 0;
	}
}