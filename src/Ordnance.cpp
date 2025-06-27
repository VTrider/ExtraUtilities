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

#include "LuaHelpers.h"
#include "Ordnance.h"

#include <lua.hpp>

#include <format>

namespace ExtraUtilities::Lua::Ordnance
{
	int BuildOrdnance(lua_State* L)
	{
		std::string requestedOrd = luaL_checkstring(L, 1);
		if (!ordnanceMap.contains(requestedOrd))
		{
			return luaL_argerror(L, 1, std::format("Could not find ordnance class for {} (doesn't exist)", requestedOrd).c_str());
		}

		BZR::Mat3 matrix = CheckMatrix(L, 2);
		BZR::handle ownerHandle = CheckHandle(L, 3);
		BZR::GameObject* ownerObj = BZR::GameObject::GetObj(ownerHandle);

		BZR::OrdnanceClass* classToBuild = ordnanceMap.at(requestedOrd);
		BZR::Ordnance* ord = BZR::OrdnanceClass::Build(classToBuild, &matrix, ownerObj->obj);

		lua_pushlightuserdata(L, ord);

		return 1;
	}

	int GetOrdnanceAttribute(lua_State* L)
	{
		if (!lua_isuserdata(L, 1))
		{
			luaL_typerror(L, 1, "Ordnance Handle");
		}

		BZR::Ordnance* ord = reinterpret_cast<BZR::Ordnance*>(lua_touserdata(L, 1));

		AttributeCode code = static_cast<AttributeCode>(luaL_checkinteger(L, 2));

		switch (code)
		{
		case ODF:
			lua_pushstring(L, ord->ordnanceClass->odf);
			break;
		case TRANSFORM:
			PushMatrix(L, ord->obj->transform);
			break;
		case INIT_TRANSFORM:
			PushMatrix(L, ord->initMat);
			break;
		case OWNER:
		{
			BZR::OBJ76* ownerObj = ord->owner;
			BZR::handle handle = BZR::GameObject::GetHandle(ownerObj->owner);
			lua_pushlightuserdata(L, reinterpret_cast<void*>(handle));
			break;
		}
		case INIT_TIME:
			lua_pushnumber(L, ord->initTime);
			break;
		default:
			return luaL_argerror(L, 2, "Invalid ordnance attribute code");
		}

		return 1;
	}

	int GetCoeffBallistic(lua_State* L)
	{
		lua_pushnumber(L, coeffBallistic.Read());
		return 1;
	}

	int SetCoeffBallistic(lua_State* L)
	{
		float newCoeff = static_cast<float>(luaL_checknumber(L, 1));
		coeffBallistic.Write(newCoeff);
		return 0;
	}
}