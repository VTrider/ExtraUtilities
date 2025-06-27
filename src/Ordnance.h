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
#include "VectorSpider.h"

#include <lua.hpp>

#include <string>
#include <unordered_map>
#include <fstream>
namespace ExtraUtilities::Lua::Ordnance
{
	inline Scanner coeffBallistic(BZR::Ordnance::coeffBallistic);

	enum AttributeCode
	{
		ODF,
		TRANSFORM,
		INIT_TRANSFORM,
		OWNER,
		INIT_TIME
	};

	int BuildOrdnance(lua_State* L);
	int GetOrdnanceAttribute(lua_State* L);

	int GetCoeffBallistic(lua_State* L);
	int SetCoeffBallistic(lua_State* L);
}