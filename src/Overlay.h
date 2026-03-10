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

#pragma once

#include <lua.hpp>

namespace ExtraUtilities::Lua::Overlay
{
	int CreateOverlay(lua_State* L);
	int DestroyOverlay(lua_State* L);
	int ShowOverlay(lua_State* L);
	int HideOverlay(lua_State* L);
	int SetOverlayZOrder(lua_State* L);
	int SetOverlayScroll(lua_State* L);

	int CreateOverlayElement(lua_State* L);
	int DestroyOverlayElement(lua_State* L);
	int HasOverlayElement(lua_State* L);
	int AddOverlay2D(lua_State* L);
	int RemoveOverlay2D(lua_State* L);
	int AddOverlayElementChild(lua_State* L);
	int RemoveOverlayElementChild(lua_State* L);
	int ShowOverlayElement(lua_State* L);
	int HideOverlayElement(lua_State* L);
	int SetOverlayMetricsMode(lua_State* L);
	int SetOverlayPosition(lua_State* L);
	int SetOverlayDimensions(lua_State* L);
	int SetOverlayMaterial(lua_State* L);
	int SetOverlayColor(lua_State* L);
	int SetOverlayCaption(lua_State* L);
	int SetOverlayTextFont(lua_State* L);
	int SetOverlayTextCharHeight(lua_State* L);
}
