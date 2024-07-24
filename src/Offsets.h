/* Copyright (C) 2023-2024 VTrider
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

#include "bzr.h"
#include <iostream>

// BZR 2.2.301

// Notes: positions will always be a VECTOR_3D unless otherwise specified,
// and numbers will always be floats, type is defined when you call the
// functions in Memory class

namespace Flags
{
	constexpr std::uintptr_t inGame = 0x0091552A; // char static address maybe not 100% stable
}

namespace Hooks
{
	constexpr std::uintptr_t weaponMask = 0x0060A8C6;
	constexpr std::uintptr_t ordnanceVelocity = 0x004803D4;
	constexpr std::uintptr_t ordnancePosition = 0x00480357;
	constexpr std::uintptr_t shotConvergence = 0x004eb590;
	constexpr std::uintptr_t freeCursor = 0x0043510E;
}

namespace Environment
{
	constexpr std::uintptr_t gravity = 0x00871A80;
}

namespace Reticle
{
	constexpr std::uintptr_t angle = 0x025CE714; 
	constexpr std::uintptr_t position = 0x025CE79C;
	constexpr std::uintptr_t range = 0x00886B20;
}

namespace Satellite
{
	constexpr std::uintptr_t state = 0x008E8F9C; // char - old value bugged in MP 0x00917AF8
	constexpr std::uintptr_t cursorPos = 0x009C9194;
	constexpr std::uintptr_t camPos = 0x009C91B4;
	constexpr std::uintptr_t clickPos = 0x009C9188; 
	constexpr std::uintptr_t panSpeed = 0x009C91D0;
	constexpr std::uintptr_t minZoom = 0x00872400;
	constexpr std::uintptr_t maxZoom = 0x008723F4;
	constexpr std::uintptr_t zoom = 0x009C91B0;
}

namespace Radar
{
	constexpr std::uintptr_t state = 0x008EAAAC; // char
}

namespace Camera
{
	constexpr std::uintptr_t zoomFactorFPP = 0x008EAD10;
	constexpr std::uintptr_t zoomFactorGlobal = 0x008EAB10;
	constexpr std::uintptr_t maxZoomFactor = 0x008A2688;
	constexpr std::uintptr_t minZoomFactor = 0x008A25FC;
	constexpr std::uintptr_t viewFrustum = 0x008EABE0; // frustum
}

namespace Misc
{
	constexpr std::uintptr_t steam64 = 0x0260B1D0; // long long (8 bytes)
	constexpr std::uintptr_t lives = 0x008E8D04; // int
	constexpr std::uintptr_t difficulty = 0x25CFA1C; // char
}