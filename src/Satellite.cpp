/* Copyright (C) 2023-2026 VTrider
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

#include "Satellite.h"

#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::Satellite
{
	int GetState(lua_State* L)
	{
		lua_pushboolean(L, state.Read());
		return 1;
	}

	int GetCursorPos(lua_State* L)
	{
		BZR::VECTOR_3D pos = cursorPos.Read();

		PushVector(L, pos);

		return 1;
	}

	int GetCameraPos(lua_State* L)
	{
		BZR::VECTOR_3D pos = camPos.Read();

		PushVector(L, pos);

		return 1;
	}

	int GetClickPos(lua_State* L)
	{
		BZR::VECTOR_3D pos = clickPos.Read();

		PushVector(L, pos);

		return 1;
	}

	int GetPanSpeed(lua_State* L)
	{
		lua_pushnumber(L, panSpeed.Read());
		return 1;
	}

	int SetPanSpeed(lua_State* L)
	{
		float speed = static_cast<float>(luaL_checknumber(L, 1));
		panSpeed.Write(speed);
		return 0;
	}

	int GetMinZoom(lua_State* L)
	{
		lua_pushnumber(L, minZoom.Read());
		return 1;
	}

	int SetMinZoom(lua_State* L)
	{
		float newMin = static_cast<float>(luaL_checknumber(L, 1));
		minZoom.Write(newMin);
		return 0;
	}

	int GetMaxZoom(lua_State* L)
	{
		lua_pushnumber(L, maxZoom.Read());
		return 1;
	}

	int SetMaxZoom(lua_State* L)
	{
		float newMax = static_cast<float>(luaL_checknumber(L, 1));
		maxZoom.Write(newMax);
		return 0;
	}

	int GetZoom(lua_State* L)
	{
		lua_pushnumber(L, zoom.Read());
		return 1;
	}

	int SetZoom(lua_State* L)
	{
		float newZoom = static_cast<float>(luaL_checknumber(L, 1));
		zoom.Write(newZoom);
		return 0;
	}
}