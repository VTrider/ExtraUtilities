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
#include "Offset.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Satellite
{
	inline Offset state(BZR::Satellite::state);
	inline Offset cursorPos(BZR::Satellite::cursorPos);
	inline Offset camPos(BZR::Satellite::camPos);
	inline Offset clickPos(BZR::Satellite::clickPos);
	inline Offset panSpeed(BZR::Satellite::panSpeed);
	inline Offset minZoom(BZR::Satellite::minZoom);
	inline Offset maxZoom(BZR::Satellite::maxZoom);
	inline Offset zoom(BZR::Satellite::zoom);

	int GetState(lua_State* L);
	int GetCursorPos(lua_State* L);
	int GetCameraPos(lua_State* L);
	int GetClickPos(lua_State* L);
	int GetPanSpeed(lua_State* L);
	int SetPanSpeed(lua_State* L);
	int GetMinZoom(lua_State* L);
	int SetMinZoom(lua_State* L);
	int GetMaxZoom(lua_State* L);
	int SetMaxZoom(lua_State* L);
	int GetZoom(lua_State* L);
	int SetZoom(lua_State* L);
}