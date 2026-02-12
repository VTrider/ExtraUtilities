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
#include "InlinePatch.h"
#include "Scanner.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::Multiplayer
{
	inline const Scanner isNetGame(BZR::Multiplayer::isNetGame, BasicScanner::Restore::DISABLED);
	inline Scanner lives(BZR::Multiplayer::lives);
	inline InlinePatch buildObjectAlwaysAsync(0x005C833D, BasicPatch::NOP, 11, BasicPatch::Status::INACTIVE);
	inline InlinePatch buildObjectAlwaysSync(0x005C833B, BasicPatch::NOP, 2, BasicPatch::Status::INACTIVE);
	inline const Scanner myNetID(BZR::Multiplayer::myNetID, BasicScanner::Restore::DISABLED);
	inline Scanner showScoreboard(BZR::Multiplayer::showScoreboard);
	// Jumps over the section of code that spawns the starting recycler in MultSTMission
	inline InlinePatch skipStartingRecycler(0x0056F014, { 0xE9, 0xBC, 0x00, 0x00, 0x00 }, BasicPatch::Status::INACTIVE);

	int BuildAsyncObject(lua_State* L);
	int BuildSyncObject(lua_State* L);

	int GetLives(lua_State* L);
	int SetLives(lua_State* L);

	int GetMyNetID(lua_State* L);

	int GetShowScoreboard(lua_State* L);
	int SetShowScoreboard(lua_State* L);

	int DisableStartingRecycler(lua_State*);
}