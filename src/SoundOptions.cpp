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

#include "SoundOptions.h"

namespace ExtraUtilities::Lua::SoundOptions
{
	int GetMusicVolume(lua_State* L)
	{
		lua_pushnumber(L, musicVolume.Read());
		return 1;
	}

	int GetEffectsVolume(lua_State* L)
	{
		lua_pushnumber(L, sfxVolume.Read());
		return 1;
	}

	int SetEffectsVolume(lua_State* L)
	{
		int newVolume = luaL_checkinteger(L, 1);
		if (newVolume < 0 || newVolume > 10)
		{
			luaL_argerror(L, 1, "Value must be between 0 and 10");
		}
		sfxVolume.Write(newVolume);
		return 0;
	}

	int GetVoiceVolume(lua_State* L)
	{
		lua_pushnumber(L, voiceVolume.Read());
		return 1;
	}

	int SetVoiceVolume(lua_State* L)
	{
		int newVolume = luaL_checkinteger(L, 1);
		if (newVolume < 0 || newVolume > 10)
		{
			luaL_argerror(L, 1, "Value must be between 0 and 10");
		}
		voiceVolume.Write(newVolume);
		return 0;
	}
}