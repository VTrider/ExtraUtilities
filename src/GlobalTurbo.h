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

#include <lua.hpp>

#include <cstdint>

namespace ExtraUtilities::Patch
{
	// this instruction compares some value in a hovercraft to an arbitrary "tolerance"
	// that is part of what determines when to use turbo, we can't replace the value it compares to
	// because it is used by a ton of other stuff and messes up the game, so this patch
	// puts the address of our own variable into the comparison
	constexpr std::uintptr_t comissPatch = 0x00601CA3;
	inline float patchedTurboTolerance = 0.9f;
	// this needs to be a double pointer because we need to put the address into the machine code
	// and memcpy will take a pointer to the pointer to the value
	inline float* p_tolerance = &patchedTurboTolerance;

	// this instruction bypasses the final check to prevent turbo from happening, so
	// when combined with a reduced tolerance it forces ai to turbo everywhere
	constexpr std::uintptr_t turboConditionPatch = 0x00601CB5;
}

namespace ExtraUtilities::Lua::Patches
{
	int GetGlobalTurbo(lua_State* L);
	int SetGlobalTurbo(lua_State* L);
}