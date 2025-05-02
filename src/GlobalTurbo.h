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

#pragma once

#include "BZR.h"

#include <lua.hpp>

#include <cstdint>
#include <unordered_map>

namespace ExtraUtilities::Patch
{
	// this instruction compares some value in a hovercraft to an arbitrary "tolerance"
	// that is part of what determines when to use turbo, we can't replace the value it compares to
	// because it is used by a ton of other stuff and messes up the game, so this patch
	// puts the address of our own variable into the comparison
	constexpr uintptr_t comissPatch = 0x00601CA3;
	inline float patchedTurboTolerance = 0.9f;

	// this instruction bypasses the final check to prevent turbo from happening, so
	// when combined with a reduced tolerance it forces ai to turbo everywhere
	constexpr uintptr_t turboConditionPatch = 0x00601CB5;

	constexpr uintptr_t turboPatchBeginAddr = 0x00601C92;
	constexpr uintptr_t turboPatchEndAddr = 0x00601CCD;

	inline bool globalTurboEnabled = false;

	inline std::unordered_map<BZR::handle, bool> setTurboUnits;
}

namespace ExtraUtilities::Lua::Patches
{
	int GetGlobalTurbo(lua_State* L);
	int SetGlobalTurbo(lua_State* L);
	int GetUnitTurbo(lua_State* L);
	int SetUnitTurbo(lua_State* L);
}