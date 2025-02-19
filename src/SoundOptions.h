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

namespace ExtraUtilities::Lua::SoundOptions
{
	using namespace BZR::SoundOptions;

	inline Scanner musicVolume(soundStruct1, { musicOffset }, BasicScanner::Restore::DISABLED); // only a display value
	// inline Scanner sfxVolume(soundStruct2, { sfxOffset }, BasicScanner::Restore::DISABLED);
	// inline Scanner voiceVolume(soundStruct2, { voiceOffset }, BasicScanner::Restore::DISABLED);

	int GetMusicVolume(lua_State* L);
	//int GetEffectsVolume(lua_State* L);
	//int SetEffectsVolume(lua_State* L);
	//int GetVoiceVolume(lua_State* L);
	//int SetVoiceVolume(lua_State* L);
}