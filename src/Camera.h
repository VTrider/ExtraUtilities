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

namespace ExtraUtilities::Lua::Camera
{
	inline const Scanner mainCam(BZR::Camera::View_Record_MainCam, BasicScanner::Restore::DISABLED);

	inline Scanner zoomFPP(BZR::Camera::zoomFactorFPP);
	inline Scanner zoomTPP(BZR::Camera::zoomFactorTPP);
	inline Scanner minZoom(BZR::Camera::minZoomFactor);
	inline Scanner maxZoom(BZR::Camera::maxZoomFactor);
	inline const Scanner userEntity(BZR::GameObject::user_entity_ptr, BasicScanner::Restore::DISABLED);
	// restore needs to be disabled to prevent camera state being saved after leaving a game
	inline Scanner currentView(BZR::Camera::currentView, BasicScanner::Restore::DISABLED);

	int GetOrigins(lua_State* L);

	int GetTransformMatrix(lua_State* L);
	int GetViewMatrix(lua_State* L);

	int GetMaxZoom(lua_State* L);
	int SetMaxZoom(lua_State* L);

	int GetMinZoom(lua_State* L);
	int SetMinZoom(lua_State* L);

	int GetView(lua_State* L);
	int SetView(lua_State* L);

	int GetZoom(lua_State* L);
	int SetZoom(lua_State* L);
}