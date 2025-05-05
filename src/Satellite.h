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
#include "Scanner.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Satellite
{
	inline const Scanner state(BZR::Satellite::state);
	inline const Scanner cursorPos(BZR::Satellite::cursorPos);
	inline const Scanner camPos(BZR::Satellite::camPos);
	inline const Scanner clickPos(BZR::Satellite::clickPos);
	inline Scanner panSpeed(BZR::Satellite::panSpeed);
	inline Scanner minZoom(BZR::Satellite::minZoom);
	inline Scanner maxZoom(BZR::Satellite::maxZoom);
	inline Scanner zoom(BZR::Satellite::zoom);

	int GetCameraPos(lua_State* L);
	int GetClickPos(lua_State* L);
	int GetCursorPos(lua_State* L);
	int GetMaxZoom(lua_State* L);
	int SetMaxZoom(lua_State* L);
	int GetMinZoom(lua_State* L);
	int SetMinZoom(lua_State* L);
	int GetPanSpeed(lua_State* L);
	int SetPanSpeed(lua_State* L);
	int GetState(lua_State* L);
	int GetZoom(lua_State* L);
	int SetZoom(lua_State* L);
}