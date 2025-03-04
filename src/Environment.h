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
#include "Ogre.h"
#include "Scanner.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Environment
{
	inline Scanner gravity(BZR::Environment::gravityVector);
	inline Scanner fog(Ogre::GetFog(), BasicScanner::Restore::DISABLED);

	// We need to store these in order to hook the function that constantly
	// resets the sunlight color whenever an explosion or other various things happen.
	// Initialize them to the map default.
	inline Ogre::Color desiredSunAmbient = *Ogre::GetAmbientLight(Ogre::sceneManager.Read());
	inline Ogre::Color desiredSunDiffuse = *Ogre::GetDiffuseColor(Ogre::terrain_masterlight.Read());
	inline Ogre::Color desiredSunSpecular = *Ogre::GetSpecularColor(Ogre::terrain_masterlight.Read());

	int GetFog(lua_State* L);
	int SetFog(lua_State* L);

	int GetGravity(lua_State* L);
	int SetGravity(lua_State* L);

	int GetSunAmbient(lua_State* L);
	int SetSunAmbient(lua_State* L);

	int GetSunDiffuse(lua_State* L);
	int SetSunDiffuse(lua_State* L);
}

namespace ExtraUtilities::Patch
{
	// Addresses for the reset function to use our own values
	constexpr uintptr_t sunAmbientReset = 0x0067DE3D;
	constexpr uintptr_t sunDiffuseReset = 0x0067DED7;
	constexpr uintptr_t sunSpecularReset = 0x0067DF53;
}