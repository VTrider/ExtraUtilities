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

#include "BZR.h"
#include "Ogre.h"
#include "Scanner.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Environment
{
	inline Scanner gravity(BZR::Environment::gravityVector);
	inline Scanner fog(Ogre::GetFog(), BasicScanner::Restore::DISABLED);

	int GetFog(lua_State* L);
	int SetFog(lua_State* L);

	int GetGravity(lua_State* L);
	int SetGravity(lua_State* L);

	int GetSunAmbient(lua_State* L);
	int SetSunAmbient(lua_State* L);
	int GetAmbientLight(lua_State* L);
	int SetAmbientLight(lua_State* L);

	int GetSunDiffuse(lua_State* L);
	int SetSunDiffuse(lua_State* L);

	int GetSunSpecular(lua_State* L);
	int SetSunSpecular(lua_State* L);

	int GetSunDirection(lua_State* L);
	int SetSunDirection(lua_State* L);

	int GetSunPowerScale(lua_State* L);
	int SetSunPowerScale(lua_State* L);

	int GetSunShadowFarDistance(lua_State* L);
	int SetSunShadowFarDistance(lua_State* L);

	int GetSkyBoxParams(lua_State* L);
	int GetSkyDomeParams(lua_State* L);
	int GetSkyPlaneParams(lua_State* L);
	int GetShowBoundingBoxes(lua_State* L);
	int SetShowBoundingBoxes(lua_State* L);
	int GetShowDebugShadows(lua_State* L);
	int SetShowDebugShadows(lua_State* L);
	int GetViewportShadowsEnabled(lua_State* L);
	int SetViewportShadowsEnabled(lua_State* L);
	int GetSceneVisibilityMask(lua_State* L);
	int SetSceneVisibilityMask(lua_State* L);

	int HasSkyBoxNode(lua_State* L);
	int HasSkyDomeNode(lua_State* L);
	int HasSkyPlaneNode(lua_State* L);

	Ogre::Color DefaultSunColor();
}

namespace ExtraUtilities::Patch
{
	constexpr uintptr_t fogReset = 0x00683370;

	void TryInitializeOgre();
}
