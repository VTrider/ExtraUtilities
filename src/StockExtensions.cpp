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

/*
 * Restored stock lua functions and C++ functions that exist in game
 * but are not exposed to scripting
 */

#include "StockExtensions.h"

#include "BZR.h"
#include "Camera.h"
#include "LuaHelpers.h"

#include <format>

namespace ExtraUtilities::Lua::StockExtensions
{
	int DoString(lua_State* L)
	{
		const char* str = luaL_checkstring(L, 1);
		luaL_dostring(L, str);
		return 0;
	}

	int MatrixInverse(lua_State* L)
	{
		BZR::MAT_3D mat = CheckMatrix(L, 1);

		BZR::MAT_3D result;
		BZR::Matrix_Inverse(&result, &mat);

		PushMatrix(L, result);

		return 1;
	}

	int ScreenToWorld(lua_State* L)
	{
		BZR::BZR_Camera* cam = Camera::mainCam.Get();

		int screenX = luaL_checkinteger(L, 1);
		int screenY = luaL_checkinteger(L, 2);

		static int actualWidth = GetSystemMetrics(SM_CXSCREEN);
		static int actualHeight = GetSystemMetrics(SM_CYSCREEN);

		if (screenX < 0 || screenX > actualWidth)
		{
			luaL_argerror(L, 1, std::format("Requested pixel coordinate {} is out of bounds", screenX).c_str());
		}
		if (screenY < 0 || screenY > actualHeight)
		{
			luaL_argerror(L, 2, std::format("Requested pixel coordinate {} is out of bounds", screenY).c_str());
		}

		float viewX = (screenX - cam->Orig_x) / cam->Const_x;
		float viewY = (screenY - cam->Orig_y) / cam->Const_y;

		BZR::VECTOR_3D cameraDirection{ viewX, viewY, 1.0f };
		BZR::VECTOR_3D worldDirection;

		BZR::Vector_Unrotate(&worldDirection, &cameraDirection, &cam->Matrix);
		worldDirection.Normalize();

		PushVector(L, worldDirection);

		return 1;
	}

	int VectorUnrotate(lua_State* L)
	{
		BZR::VECTOR_3D vec = CheckVectorOrSingles(L, 1);
		BZR::MAT_3D perspectiveMat = CheckMatrix(L, 2);

		BZR::VECTOR_3D result;
		BZR::Vector_Unrotate(&result, &vec, &perspectiveMat);

		PushVector(L, result);

		return 1;
	}
}