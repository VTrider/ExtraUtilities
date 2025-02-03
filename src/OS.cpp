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

#include "OS.h"

#pragma push_macro("MessageBox")
#undef MessageBox

namespace ExtraUtilities::Lua::OS
{
	int MessageBox(lua_State* L)
	{
		const char* message = luaL_checkstring(L, 1);
		MessageBoxA(0, message, "Extra Utilities", MB_OK | MB_APPLMODAL);
		return 0;
	}

	int GetScreenResolution(lua_State* L)
	{
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		lua_pushnumber(L, width);
		lua_pushnumber(L, height);

		return 2;
	}
}

#pragma pop_macro("MessageBox")