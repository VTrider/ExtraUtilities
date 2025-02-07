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

#include <bzr.h>
#include "bzfunc.h"

#include <Windows.h>

#include <iostream>
#include <vector>



namespace Hooks // memory addresses of functions of interest
{
	constexpr std::uintptr_t weaponMask       = 0x0060A8C6;
	constexpr std::uintptr_t ordnanceVelocity = 0x004803D4;
	constexpr std::uintptr_t ordnancePosition = 0x00480357;
	constexpr std::uintptr_t shotConvergence  = 0x004eb590;
	constexpr std::uintptr_t freeCursor       = 0x0043510E;
	constexpr std::uintptr_t getLightPtr      = 0x0067FB13;
	constexpr std::uintptr_t playOption       = 0x005C18B7;
	constexpr std::uintptr_t selectNone       = 0x004A6D50;
	constexpr std::uintptr_t bulletHitCB      = 0x00480774;
	constexpr std::uintptr_t playerWeapons    = 0x00417F9A;
	constexpr std::uintptr_t scrapGain        = 0x005b831a;
}

namespace Misc
{
	constexpr std::uintptr_t difficulty = 0x25CFA1C; // char
	constexpr std::uintptr_t coeffMortar = 0x008A2858; // float
	constexpr std::uintptr_t turbo = 0x00601CB5;
}