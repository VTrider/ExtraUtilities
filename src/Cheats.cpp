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

#include "Cheats.h"

#include "Hook.h"
#include "bzr.h"

namespace ExtraUtilities::Patch
{
	static void __declspec(naked) InfiniteAmmoCallback()
	{
		__asm
		{
			// Force eax to be 10000 (xor'd with 0x33333333)
			mov eax, 10000
			xor eax, 0x33333333
			ret
		}
	}

	static void __declspec(naked) InfiniteScrapCallback()
	{
		__asm
		{
			mov eax, 10000
			xor eax, 0x33333333
			ret
		}
	}

	Hook infiniteAmmoHook(BZR::Cheats::InfiniteAmmoAddr, &InfiniteAmmoCallback, 8, BasicPatch::Status::INACTIVE);
	Hook infiniteScrapHook(BZR::Cheats::InfiniteScrapAddr, &InfiniteScrapCallback, 8, BasicPatch::Status::INACTIVE);
}

namespace ExtraUtilities::Lua::Patches
{
	int GetInfiniteAmmo(lua_State* L)
	{
		lua_pushboolean(L, Patch::infiniteAmmoHook.IsActive());
		return 1;
	}

	int SetInfiniteAmmo(lua_State* L)
	{
		bool enable = lua_toboolean(L, 1);
		Patch::infiniteAmmoHook.SetStatus(enable);
		return 0;
	}

	int GetInfiniteScrap(lua_State* L)
	{
		lua_pushboolean(L, Patch::infiniteScrapHook.IsActive());
		return 1;
	}

	int SetInfiniteScrap(lua_State* L)
	{
		bool enable = lua_toboolean(L, 1);
		Patch::infiniteScrapHook.SetStatus(enable);
		return 0;
	}
}
