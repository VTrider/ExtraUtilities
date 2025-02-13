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

namespace ExtraUtilities::Lua::GameObject
{
	int GetHandle(lua_State* L);
	int GetObj(lua_State* L);
	int SetAsUser(lua_State* L);

	int GetRadarRange(lua_State* L);
	int SetRadarRange(lua_State* L);
	int GetRadarPeriod(lua_State* L);
	int SetRadarPeriod(lua_State* L);

	int GetVelocJam(lua_State* L);
	int SetVelocJam(lua_State* L);

	int SetHeadlightDiffuse(lua_State* L);
	int SetHeadlightSpecular(lua_State* L);
	int SetHeadlightRange(lua_State* L);
	int SetHeadlightVisible(lua_State* L);
}