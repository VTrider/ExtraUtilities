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

#include "GlobalTurbo.h"

#include "InlinePatch.h"
#include "LuaHelpers.h"

namespace ExtraUtilities::Patch
{
	InlinePatch turboPatch1(comissPatch, &patchedTurboTolerance, InlinePatch::Status::INACTIVE);
	InlinePatch turboPatch2(turboConditionPatch, BasicPatch::NOP, 2, InlinePatch::Status::INACTIVE);
}

namespace ExtraUtilities::Lua::Patches
{
	int GetGlobalTurbo(lua_State* L)
	{
		if (Patch::turboPatch1.IsActive() && Patch::turboPatch2.IsActive())
		{
			lua_pushboolean(L, true);
		}
		else
		{
			lua_pushboolean(L, false);
		}

		return 0;
	}

	int SetGlobalTurbo(lua_State* L)
	{
		bool state = CheckBool(L, 1);

		if (state == true)
		{
			Patch::turboPatch1.Reload();
			Patch::turboPatch2.Reload();
		}
		else
		{
			Patch::turboPatch1.Unload();
			Patch::turboPatch2.Unload();
		}

		return 0;
	}
}