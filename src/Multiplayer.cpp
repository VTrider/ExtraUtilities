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

#include "Multiplayer.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Multiplayer
{
	int BuildAsyncObject(lua_State* L)
	{
		buildObjectAlwaysAsync.Reload();

		// Set up stack for wrappuh call
		lua_getglobal(L, "BuildObject");
		
		// Handle the variadic args to BuildObject,
		// the function needs to be first so we shuffle
		// it's args to be on top
		int argC = lua_gettop(L);
		for (int i = 1; i < argC ; i++) // remember 1 based index!
		{
			lua_pushvalue(L, i);
		}

		lua_call(L, argC - 1, 1); // minus one because one of the args is the function to be called

		buildObjectAlwaysAsync.Unload();
		return 1;
	}

	int BuildSyncObject(lua_State* L)
	{
		buildObjectAlwaysSync.Reload();

		// Set up stack for wrappuh call
		lua_getglobal(L, "BuildObject");

		// Handle the variadic args to BuildObject,
		// the function needs to be first so we shuffle
		// it's args to be on top
		int argC = lua_gettop(L);
		for (int i = 1; i < argC; i++) // remember 1 based index!
		{
			lua_pushvalue(L, i);
		}

		lua_call(L, argC - 1, 1); // minus one because one of the args is the function to be called

		buildObjectAlwaysSync.Unload();
		return 1;
	}

	int GetLives(lua_State* L)
	{
		lua_pushinteger(L, lives.Read());
		return 1;
	}

	int SetLives(lua_State* L)
	{
		int newLives = luaL_checkinteger(L, 1);
		lives.Write(newLives);

		// This will crash if done in singleplayer,
		// not like lives do anything in SP anyways tho
		if (isNetGame.Read() == true)
		{
			BZR::Multiplayer::UpdateLives();
		}
		
		return 0;
	}

	int GetMyNetID(lua_State* L)
	{
		lua_pushinteger(L, myNetID.Read());
		return 1;
	}
}