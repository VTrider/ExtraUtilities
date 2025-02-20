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

#undef GetObject // windows.h name conflict

namespace ExtraUtilities::Lua::Reticle
{
	inline Scanner position(BZR::Reticle::position);
	inline Scanner range(BZR::Reticle::range);
	inline Scanner object(BZR::Reticle::object);
	inline Scanner matrix(BZR::Reticle::matrix);

	int GetMatrix(lua_State* L);
	int GetObject(lua_State* L);
	int GetPosition(lua_State* L);
	int GetRange(lua_State* L);
	int SetRange(lua_State* L);
}