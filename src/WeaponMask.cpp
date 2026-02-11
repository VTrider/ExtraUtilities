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

#include "WeaponMask.h"

#include "Hook.h"
#include "bzr.h"

namespace ExtraUtilities::Patch
{
	static uint32_t lastWeaponMask = 0;

	static void __declspec(naked) WeaponMaskCallback()
	{
		__asm
		{
			mov [lastWeaponMask], edx
			
			// Replicate original code
			mov [ecx+0x1C], edx
			mov eax, [ebp-0x110]
			
			ret
		}
	}

	Hook weaponMaskHook(BZR::Cheats::WeaponMaskCaptureAddr, &WeaponMaskCallback, 9, BasicPatch::Status::ACTIVE);

	uint32_t GetCapturedWeaponMask()
	{
		return lastWeaponMask;
	}
}

namespace ExtraUtilities::Lua::Patches
{
	int GetWeaponMask(lua_State* L)
	{
		lua_pushinteger(L, Patch::GetCapturedWeaponMask());
		return 1;
	}
}
