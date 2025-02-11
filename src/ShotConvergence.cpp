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

#include "ShotConvergence.h"

#include "InlinePatch.h"
#include "LuaHelpers.h"

namespace ExtraUtilities::Patch
{
	InlinePatch shotConvergence(wingmanWeaponAimVftableEntry, &walkerUpdateWeaponAim, 4, InlinePatch::Status::INACTIVE);
}

namespace ExtraUtilities::Lua::Patches
{
	int GetShotConvergence(lua_State* L)
	{
		lua_pushboolean(L, Patch::shotConvergence.IsActive());
		return 1;
	}

	int SetShotConvergence(lua_State* L)
	{
		bool state = CheckBool(L, 1);
		if (state == true)
		{
			Patch::shotConvergence.Reload();
		}
		else
		{
			Patch::shotConvergence.Unload();
		}
		return 0;
	}
}