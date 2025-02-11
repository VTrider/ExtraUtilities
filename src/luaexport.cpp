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

/*--------------------------------
* Exported lua functions go here *
--------------------------------*/

#include "About.h"
#include "Exports.h"
#include "LuaState.h"
#include "Patches.h"

#include "lua.hpp"
#include <Windows.h>

#include <thread> 

// Avoid name collision with winapi macro
#pragma push_macro("MessageBox")
#undef MessageBox

namespace ExtraUtilities::Lua
{
	// MUST BE EXECUTED WHEN GARBAGE COLLECTOR IS PAUSED! See note at bottom.
	int Init(lua_State* L)
	{
		state = L; // save the state pointer to use in callbacks

		// Register all this stuff inside the library table
		lua_getglobal(L, "exu");

		int exuIdx = lua_gettop(L);

		// Version string
		lua_pushstring(L, version.c_str());
		lua_setfield(L, exuIdx, "VERSION");

		// Difficulty enum
		lua_newtable(L);

		lua_pushinteger(L, 0);
		lua_setfield(L, -2, "VERY_EASY");

		lua_pushinteger(L, 1);
		lua_setfield(L, -2, "EASY");

		lua_pushinteger(L, 2);
		lua_setfield(L, -2, "MEDIUM");

		lua_pushinteger(L, 3);
		lua_setfield(L, -2, "HARD");

		lua_pushinteger(L, 4);
		lua_setfield(L, -2, "VERY_HARD");

		// Set the field inside the library table
		lua_setfield(L, exuIdx, "DIFFICULTY");

		// Radar state enum
		lua_newtable(L);

		lua_pushinteger(L, 0);
		lua_setfield(L, -2, "MINIMAP");

		lua_pushinteger(L, 1);
		lua_setfield(L, -2, "RADAR");

		lua_setfield(L, exuIdx, "RADAR");

		// Satellite state enum
		lua_newtable(L);

		lua_pushinteger(L, 0);
		lua_setfield(L, -2, "DISABLED");

		lua_pushinteger(L, 1);
		lua_setfield(L, -2, "ENABLED");

		lua_setfield(L, exuIdx, "SATELLITE");

		// VTnoia lua pop in case I missed something before
		lua_pop(L, -1);

		return 0;
	}

	extern "C"
	{
		int __declspec(dllexport) luaopen_exu(lua_State* L)
		{
			const luaL_Reg EXPORT[] = {
				// Control Panel
				{ "SelectAdd",  &ControlPanel::SelectAdd },
				{ "SelectNone", &ControlPanel::SelectNone },
				{ "SelectOne",  &ControlPanel::SelectOne },

				// Environment
				{ "GetGravity",    &Environment::GetGravity },
				{ "SetGravity",    &Environment::SetGravity },
				{ "GetFog",		   &Environment::GetFog },
				{ "SetFog",		   &Environment::SetFog },
				{ "GetSunAmbient", &Environment::GetSunAmbient },
				{ "SetSunAmbient", &Environment::SetSunAmbient },

				// GameObject
				{ "GetHandle",		&GameObject::GetHandle },
				{ "GetObj",			&GameObject::GetObj },
				{ "SetAsUser",		&GameObject::SetAsUser },
				{ "GetRadarRange",	&GameObject::GetRadarRange },
				{ "SetRadarRange",	&GameObject::SetRadarRange },
				{ "GetRadarPeriod", &GameObject::GetRadarPeriod },
				{ "SetRadarPeriod", &GameObject::SetRadarPeriod },
				{ "GetVelocJam",	&GameObject::GetVelocJam },
				{ "SetVelocJam",	&GameObject::SetVelocJam },

				// IO
				{ "GetGameKey", &IO::GetGameKey },

				// Multiplayer
				{ "GetLives", &Multiplayer::GetLives },
				{ "SetLives", &Multiplayer::SetLives },

				// Ordnance
				{ "GetCoeffBallistic", &Ordnance::GetCoeffBallistic },
				{ "SetCoeffBallistic", &Ordnance::SetCoeffBallistic },

				// OS
				{ "MessageBox",			 &OS::MessageBox },
				{ "GetScreenResolution", &OS::GetScreenResolution },

				// Patches
				{ "GetShotConvergence", &Patches::GetShotConvergence },
				{ "SetShotConvergence", &Patches::SetShotConvergence },
				{ "GetGlobalTurbo",     &Patches::GetGlobalTurbo },
				{ "SetGlobalTurbo",     &Patches::SetGlobalTurbo },

				// Play Options
				{ "GetAutoLevel",	 &PlayOption::GetAutoLevel },
				{ "SetAutoLevel",	 &PlayOption::SetAutoLevel },
				{ "GetDifficulty",   &PlayOption::GetDifficulty },
				{ "SetDifficulty",   &PlayOption::SetDifficulty },
				{ "GetTLI",			 &PlayOption::GetTLI },
				{ "SetTLI",			 &PlayOption::SetTLI },
				{ "GetReverseMouse", &PlayOption::GetReverseMouse },
				{ "SetReverseMouse", &PlayOption::SetReverseMouse },

				// Radar
				{ "GetRadarState", &Radar::GetState },
				{ "SetRadarState", &Radar::SetState },

				// Reticle
				{ "GetReticlePos",    &Reticle::GetPosition },
				{ "GetReticleRange",  &Reticle::GetRange },
				{ "SetReticleRange",  &Reticle::SetRange },
				{ "GetReticleObject", &Reticle::GetObject },
				{ "GetReticleMatrix", &Reticle::GetMatrix },

				// Satellite

				{ "GetSatState",     &Satellite::GetState },
				{ "GetSatCursorPos", &Satellite::GetCursorPos },
				{ "GetSatCameraPos", &Satellite::GetCameraPos },
				{ "GetSatClickPos",  &Satellite::GetClickPos },
				{ "GetSatPanSpeed",	 &Satellite::GetPanSpeed },
				{ "SetSatPanSpeed",	 &Satellite::SetPanSpeed },
				{ "GetSatMinZoom",	 &Satellite::GetMinZoom },
				{ "SetSatMinZoom",	 &Satellite::SetMinZoom },
				{ "GetSatMaxZoom",	 &Satellite::GetMaxZoom },
				{ "SetSatMaxZoom",	 &Satellite::SetMaxZoom },
				{ "GetSatZoom",		 &Satellite::GetZoom },
				{ "SetSatZoom",		 &Satellite::SetZoom },

				// Sound Options
				{ "GetMusicVolume",   &SoundOptions::GetMusicVolume },
				{ "GetEffectsVolume", &SoundOptions::GetEffectsVolume },
				{ "SetEffectsVolume", &SoundOptions::SetEffectsVolume },
				{ "GetVoiceVolume",   &SoundOptions::GetVoiceVolume },
				{ "SetVoiceVolume",   &SoundOptions::SetVoiceVolume },

				// Steam
				{ "GetSteam64", &Steam::GetSteam64 },

				// Stock Extensions
				{ "DoString", &StockExtensions::DoString },

				{ "Test", [](lua_State*) -> int
				{
					return 0;
				}},

				// Function register table must end with a zero entry
				{ 0, 0 }
			};

			// Ok.. this is voodoo LOL. I believe the heap corruption is caused when
			// pushing a large amount of data into the lua state triggers a reallocation,
			// so if you stop the garbage collector and resume after the library is registered
			// and initialized it will won't get corrupted.
			//
			// So: it is CRITICAL that any initialization is done while the garbage collector
			// is STOPPED (in between the lua_gc calls)

			lua_gc(L, LUA_GCSTOP, 0);

			luaL_register(L, "exu", EXPORT);
			Init(L);

			lua_gc(L, LUA_GCRESTART, 0);

			return 0;
		}
	}
}

#pragma pop_macro("MessageBox")