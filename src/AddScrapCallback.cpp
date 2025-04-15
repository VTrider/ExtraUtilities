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

#include "AddScrapCallback.h"

#include "Hook.h"
#include "LuaState.h"

namespace ExtraUtilities::Patch
{
	static void __cdecl LuaCallback(uint32_t teamNumber, uint32_t scrapAmount)
	{
		lua_State* L = Lua::state;

		lua_getglobal(L, "exu");
		lua_getfield(L, -1, "AddScrap");

		if (!lua_isfunction(L, -1))
		{
			return;
		}

		lua_pushinteger(L, teamNumber);
		lua_pushinteger(L, scrapAmount);

		lua_call(L, 2, 0);

		lua_pop(L, -1);
	}

	static void __declspec(naked) AddScrapCallback()
	{
		__asm
		{
			/*
			* Notes:
			* 
			* Team object in ecx
			* team number in ecx+0x180
			* 
			* Scrap amount to add in param 1/ebp+0x08
			*/

			// Game code - side note why tf is it doing this lol
			mov [ebp-0x04], ecx
			mov ecx, [ebp-0x04]

			pushad
			pushfd

			mov eax, [ebp+0x08] // scrap amount
			push eax
			mov ecx, [ecx+0x180] // team number
			push ecx
			call LuaCallback
			add esp, 0x08

			popfd
			popad
		
			ret
		}
	}
	Hook addScrapHook(addScrap, &AddScrapCallback, 6, BasicPatch::Status::ACTIVE);
}

namespace ExtraUtilities::Lua::Patches
{
	int AddScrapSilent(lua_State* L)
	{
		int teamNum = luaL_checkinteger(L, 1);
		int amount = luaL_checkinteger(L, 2);

		lua_getglobal(L, "AddScrap");
		lua_pushinteger(L, teamNum);
		lua_pushinteger(L, amount);

		Patch::addScrapHook.Unload();

		lua_call(L, 2, 1);
		
		Patch::addScrapHook.Reload();

		return 1;
	}
}