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

#include "KillMessages.h"

#include "Hook.h"

#include <string>
#include <unordered_map>
#include <fstream>
namespace ExtraUtilities::Patch
{
	constexpr size_t TEAM_MAX_LENGTH = 0x20;
	std::unordered_map<int, std::string> messageMap;

	static void __cdecl ProcessKillMessage(int killedTeam, int killerTeam, char* killedTeamName, char* killerTeamName)
	{
		// When a player ejects we want it to say the actual player name not the custom one, in the code an ejection will have
		// the killer team be 0, and the killer team name will be blank
		if (killerTeam == 0)
		{
			if (std::strlen(killerTeamName) == 0)
			{
				return;
			}
		}

		if (messageMap.contains(killedTeam))
		{
			std::strncpy(killedTeamName, messageMap.at(killedTeam).c_str(), TEAM_MAX_LENGTH);
		}
		if (messageMap.contains(killerTeam))
		{
			std::strncpy(killerTeamName, messageMap.at(killerTeam).c_str(), TEAM_MAX_LENGTH);
		}
	}

	static __declspec(naked) void KillMessageHook()
	{
		__asm
		{
			// Notes:
			// Killed team number at ebp+0x08
			// Killer team number at ebp+0x0C
			// Killer team name at ebp-0x4C
			// Killed team name at ebp-0x28

			pushad
			pushfd

			lea eax, [ebp-0x4C]
			push eax
			lea eax, [ebp-0x28]
			push eax
			mov eax, [ebp+0x0C]
			push eax
			mov eax, [ebp+0x08]
			push eax
			call ProcessKillMessage
			add esp, 0x10 // 4 params

			popfd
			popad


			// Game code
			mov edx, [ebp-0x58]
			mov [ebp + edx - 0x4c], 0

			ret
		}
	}
	Hook killMessageHook(0x0062627f, KillMessageHook, 8, BasicPatch::Status::ACTIVE);
}

namespace ExtraUtilities::Lua::Patches
{
	int GetCustomKillMessage(lua_State* L)
	{
		int team = luaL_checkinteger(L, 1);

		if (Patch::messageMap.contains(team))
		{
			lua_pushstring(L, Patch::messageMap.at(team).c_str());
		}
		else
		{
			lua_pushnil(L);
		}

		return 1;
	}

	int SetCustomKillMessage(lua_State* L)
	{
		int team = luaL_checkinteger(L, 1);

		if (lua_isnil(L, 2))
		{
			Patch::messageMap.erase(team);
		}
		else
		{
			size_t length{};
			std::string message = luaL_checklstring(L, 2, &length);

			if (length > Patch::TEAM_MAX_LENGTH)
			{
				return luaL_argerror(L, 2, "Extra Utilities Error: Kill message string must be under 32 characters");
			}

			Patch::messageMap.emplace(team, message);
		}

		return 0;
	}
}