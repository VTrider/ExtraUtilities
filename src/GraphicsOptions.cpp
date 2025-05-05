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

#include "GraphicsOptions.h"

#include "BZR.h"
#include "Camera.h"

#include <cmath>

namespace ExtraUtilities::Lua::GraphicsOptions
{
	int GetFullscreen(lua_State* L)
	{
		lua_pushboolean(L, isFullscreen.Read());
		return 1;
	}

	int GetGameResolution(lua_State* L)
	{
		BZR::BZR_Camera* cam = Camera::mainCam.Get();

		// We floor them here because they are floating point values
		// and sometimes the resulting value might be 1 pixel bigger
		// than your actual resolution
		int x = static_cast<int>(std::floor(cam->Orig_x)) * 2;
		int y = static_cast<int>(std::floor(cam->Orig_y)) * 2;

		lua_pushinteger(L, x);
		lua_pushinteger(L, y);

		return 2;
	}
}