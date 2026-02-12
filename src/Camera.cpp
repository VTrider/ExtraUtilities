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

#include "Camera.h"

#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::Camera
{
	int GetOrigins(lua_State* L)
	{
		BZR::BZR_Camera* cam = mainCam.Get();
		
		lua_createtable(L, 0, 4); // table with 4 non array (map) elements

		lua_pushnumber(L, cam->Orig_x);
		lua_setfield(L, -2, "Orig_x");

		lua_pushnumber(L, cam->Orig_y);
		lua_setfield(L, -2, "Orig_y");

		lua_pushnumber(L, cam->Const_x);
		lua_setfield(L, -2, "Const_x");

		lua_pushnumber(L, cam->Const_y);
		lua_setfield(L, -2, "Const_y");

		return 1;
	}

	int GetTransformMatrix(lua_State* L)
	{
		BZR::MAT_3D viewMatrix = mainCam.Get()->Matrix;
		
		BZR::MAT_3D transformMatrix;
		BZR::Matrix_Inverse(&transformMatrix, &viewMatrix);
		
		PushMatrix(L, transformMatrix);

		return 1;
	}

	int GetViewMatrix(lua_State* L)
	{
		BZR::MAT_3D viewMatrix = mainCam.Get()->Matrix;

		PushMatrix(L, viewMatrix);

		return 1;
	}

	int GetMaxZoom(lua_State* L)
	{
		lua_pushnumber(L, maxZoom.Read());
		return 1;
	}

	int SetMaxZoom(lua_State* L)
	{
		float zoom = static_cast<float>(luaL_checknumber(L, 1));
		maxZoom.Write(zoom);
		return 0;
	}

	int GetMinZoom(lua_State* L)
	{
		lua_pushnumber(L, minZoom.Read());
		return 1;
	}

	int SetMinZoom(lua_State* L)
	{
		float zoom = static_cast<float>(luaL_checknumber(L, 1));
		minZoom.Write(zoom);
		return 0;
	}

	int GetView(lua_State* L)
	{
		lua_pushinteger(L, currentView.Read());
		return 1;
	}

	int SetView(lua_State* L)
	{
		int view = luaL_checkinteger(L, 1);

		using enum BZR::Camera::View;
		switch (view)
		{
		case COCKPIT:
			break;
		case NO_COCKPIT:
			break;
		case CHASE:
			break;
		case ORBIT:
			break;
		case NO_HUD:
			break;
		case FREECAM:
			break;
		default:
			luaL_argerror(L, 1, "Extra Utilities Error: Invalid view mode");
			return 0;
		}

		BZR::Camera::Set_View(userEntity.Read(), view);

		return 0;
	}

	int GetZoom(lua_State* L)
	{
		int camera = luaL_checkinteger(L, 1);

		using enum BZR::Camera::View;
		switch (camera)
		{
		case FIRST_PERSON:
			lua_pushnumber(L, zoomFPP.Read());
			break;
		case THIRD_PERSON:
			lua_pushnumber(L, zoomTPP.Read());
			break;
		default:
			luaL_argerror(L, 1, "Extra Utilities Error: Invalid camera");
			return 0;
		}
		return 1;
	}

	int SetZoom(lua_State* L)
	{
		int camera = luaL_checkinteger(L, 1);
		float zoom = static_cast<float>(luaL_checknumber(L, 2));

		using enum BZR::Camera::View;
		switch (camera)
		{
		case FIRST_PERSON:
			zoomFPP.Write(zoom);
			break;
		case THIRD_PERSON:
			zoomTPP.Write(zoom);
			break;
		default:
			luaL_argerror(L, 1, "Extra Utilities Error: Invalid camera");
			return 0;
		}
		return 0;
	}
}