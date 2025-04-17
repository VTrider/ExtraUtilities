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
#include "LuaHelpers.h"
#include "LuaState.h"
#include "Patches.h"

#include "lua.hpp"
#include <Windows.h>

// Avoid name collision with winapi macro
#pragma push_macro("MessageBox")
#undef MessageBox

namespace ExtraUtilities::Lua
{
	int Init(lua_State* L)
	{
		state = L; // save the state pointer to use in callbacks

		// Register all this stuff inside the library table
		lua_getglobal(L, "exu");

		int exuIdx = lua_gettop(L);

		// Version string
		lua_pushstring(L, version.c_str());
		lua_setfield(L, exuIdx, "VERSION");

		// Camera enum
		lua_newtable(L);

		lua_pushinteger(L, BZR::Camera::FIRST_PERSON);
		lua_setfield(L, -2, "FIRST_PERSON");

		lua_pushinteger(L, BZR::Camera::THIRD_PERSON);
		lua_setfield(L, -2, "THIRD_PERSON");

		lua_pushinteger(L, BZR::Camera::COCKPIT);
		lua_setfield(L, -2, "COCKPIT");

		lua_pushinteger(L, BZR::Camera::NO_COCKPIT);
		lua_setfield(L, -2, "NO_COCKPIT");

		lua_pushinteger(L, BZR::Camera::CHASE);
		lua_setfield(L, -2, "CHASE");

		lua_pushinteger(L, BZR::Camera::ORBIT);
		lua_setfield(L, -2, "ORBIT");

		lua_pushinteger(L, BZR::Camera::NO_HUD);
		lua_setfield(L, -2, "NO_HUD");

		lua_pushinteger(L, BZR::Camera::EDITOR);
		lua_setfield(L, -2, "EDITOR");

		lua_pushinteger(L, BZR::Camera::CHEAT_SATELLITE);
		lua_setfield(L, -2, "CHEAT_SATELLITE");

		lua_pushinteger(L, BZR::Camera::FREECAM);
		lua_setfield(L, -2, "FREECAM");

		lua_pushinteger(L, BZR::Camera::TOGGLE_SATELLITE);
		lua_setfield(L, -2, "TOGGLE_SATELLITE");

		lua_pushinteger(L, BZR::Camera::TERRAIN_EDIT);
		lua_setfield(L, -2, "TERRAIN_EDIT");

		lua_setfield(L, -2, "CAMERA"); // end camera enum

		// Defaults enum
		lua_newtable(L);

		lua_pushnumber(L, 0.99f);
		lua_setfield(L, -2, "CAMERA_MIN_ZOOM");

		lua_pushnumber(L, 1.60f);
		lua_setfield(L, -2, "CAMERA_MAX_ZOOM");

		lua_pushnumber(L, 4.9f);
		lua_setfield(L, -2, "COEFF_BALLISTIC");

		PushVector(L, { 0.f, -9.8f, 0.f });
		lua_setfield(L, -2, "GRAVITY_VECTOR");

		lua_pushnumber(L, 200.f);
		lua_setfield(L, -2, "RETICLE_RANGE");

		lua_pushnumber(L, 8.f);
		lua_setfield(L, -2, "SAT_MAX_ZOOM");

		lua_pushnumber(L, 2.f);
		lua_setfield(L, -2, "SAT_MIN_ZOOM");

		lua_pushnumber(L, 1250.f);
		lua_setfield(L, -2, "SAT_PAN_SPEED");

		lua_setfield(L, exuIdx, "DEFAULTS"); // Set the field inside the library table

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

		lua_setfield(L, exuIdx, "DIFFICULTY");

		// Ogre
		lua_newtable(L);

		lua_newtable(L); // Default headlight color (diffuse and specular)

		lua_pushnumber(L, 1.0f);
		lua_setfield(L, -2, "R");

		lua_pushnumber(L, 1.0f);
		lua_setfield(L, -2, "G");

		lua_pushnumber(L, 1.0f);
		lua_setfield(L, -2, "B");

		lua_setfield(L, -2, "HEADLIGHT_COLOR"); // end color rable

		lua_newtable(L); // Default headlight range

		lua_pushnumber(L, 0.1745f);
		lua_setfield(L, -2, "INNER_ANGLE");

		lua_pushnumber(L, 0.3490f);
		lua_setfield(L, -2, "OUTER_ANGLE");

		lua_pushnumber(L, 1.0f);
		lua_setfield(L, -2, "FALLOFF");

		lua_setfield(L, -2, "HEADLIGHT_RANGE"); // end range table

		lua_setfield(L, -2, "OGRE"); // end ogre enum

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

	extern "C" int __declspec(dllexport) luaopen_exu(lua_State* L)
	{
		const luaL_Reg EXPORT[] = {
			// Camera
			{ "GetCameraMaxZoom", &Camera::GetMaxZoom },
			{ "SetCameraMaxZoom", &Camera::SetMaxZoom },
			{ "GetCameraMinZoom", &Camera::GetMinZoom },
			{ "SetCameraMinZoom", &Camera::SetMinZoom },
			{ "GetCameraView", &Camera::GetView },
			{ "SetCameraView", &Camera::SetView },
			{ "GetCameraZoom", &Camera::GetZoom },
			{ "SetCameraZoom", &Camera::SetZoom },

			// Control Panel
			{ "SelectAdd",  &ControlPanel::SelectAdd },
			{ "SelectNone", &ControlPanel::SelectNone },
			{ "SelectOne",  &ControlPanel::SelectOne },

			// Environment
			{ "GetFog",		   &Environment::GetFog },
			{ "SetFog",		   &Environment::SetFog },
			{ "GetGravity",    &Environment::GetGravity },
			{ "SetGravity",    &Environment::SetGravity },
			{ "GetSunAmbient", &Environment::GetSunAmbient },
			{ "SetSunAmbient", &Environment::SetSunAmbient },
			{ "GetSunDiffuse", &Environment::GetSunDiffuse },
			{ "SetSunDiffuse", &Environment::SetSunDiffuse },
			{ "GetSunSpecular", &Environment::GetSunSpecular },
			{ "SetSunSpecular", &Environment::SetSunSpecular },

			// GameObject
			{ "SetAsUser",		&GameObject::SetAsUser },
			{ "GetHandle",		&GameObject::GetHandle },
			{ "SetHeadlightDiffuse", &GameObject::SetHeadlightDiffuse },
			{ "SetHeadlightSpecular", &GameObject::SetHeadlightSpecular },
			{ "SetHeadlightRange", &GameObject::SetHeadlightRange },
			{ "SetHeadlightVisible", &GameObject::SetHeadlightVisible },
			{ "GetMass", &GameObject::GetMass },
			{ "SetMass", &GameObject::SetMass },
			{ "GetObj",			&GameObject::GetObj },
			{ "GetRadarRange",	&GameObject::GetRadarRange },
			{ "SetRadarRange",	&GameObject::SetRadarRange },
			{ "GetRadarPeriod", &GameObject::GetRadarPeriod },
			{ "SetRadarPeriod", &GameObject::SetRadarPeriod },
			{ "GetVelocJam",	&GameObject::GetVelocJam },
			{ "SetVelocJam",	&GameObject::SetVelocJam },

			// IO
			{ "GetGameKey", &IO::GetGameKey },

			// Multiplayer
			{ "BuildAsyncObject", &Multiplayer::BuildAsyncObject },
			{ "BuildSyncObject", &Multiplayer::BuildSyncObject },
			{ "GetLives", &Multiplayer::GetLives },
			{ "SetLives", &Multiplayer::SetLives },
			{ "GetMyNetID", &Multiplayer::GetMyNetID },
			{ "GetShowScoreboard", &Multiplayer::GetShowScoreboard },
			{ "SetShowScoreboard", &Multiplayer::SetShowScoreboard },

			// Ordnance
			{ "GetCoeffBallistic", &Ordnance::GetCoeffBallistic },
			{ "SetCoeffBallistic", &Ordnance::SetCoeffBallistic },

			// OS
			{ "GetScreenResolution", &OS::GetScreenResolution },
			{ "MessageBox",			 &OS::MessageBox },

			// Patches
			{ "AddScrapSilent",     &Patches::AddScrapSilent },
			{ "GetGlobalTurbo",     &Patches::GetGlobalTurbo },
			{ "SetGlobalTurbo",     &Patches::SetGlobalTurbo },
			{ "GetOrdnanceVelocInheritance", &Patches::GetOrdnanceVelocInheritance },
			{ "SetOrdnanceVelocInheritance", &Patches::SetOrdnanceVelocInheritance },
			{ "GetShotConvergence", &Patches::GetShotConvergence },
			{ "SetShotConvergence", &Patches::SetShotConvergence },

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
			{ "GetReticleMatrix", &Reticle::GetMatrix },
			{ "GetReticleObject", &Reticle::GetObject },
			{ "GetReticlePos",    &Reticle::GetPosition },
			{ "GetReticleRange",  &Reticle::GetRange },
			{ "SetReticleRange",  &Reticle::SetRange },

			// Satellite

			{ "GetSatCameraPos", &Satellite::GetCameraPos },
			{ "GetSatClickPos",  &Satellite::GetClickPos },
			{ "GetSatCursorPos", &Satellite::GetCursorPos },
			{ "GetSatMaxZoom",	 &Satellite::GetMaxZoom },
			{ "SetSatMaxZoom",	 &Satellite::SetMaxZoom },
			{ "GetSatMinZoom",	 &Satellite::GetMinZoom },
			{ "SetSatMinZoom",	 &Satellite::SetMinZoom },
			{ "GetSatPanSpeed",	 &Satellite::GetPanSpeed },
			{ "SetSatPanSpeed",	 &Satellite::SetPanSpeed },
			{ "GetSatState",     &Satellite::GetState },
			{ "GetSatZoom",		 &Satellite::GetZoom },
			{ "SetSatZoom",		 &Satellite::SetZoom },

			// Sound Options
			{ "GetMusicVolume",   &SoundOptions::GetMusicVolume },
			//{ "GetEffectsVolume", &SoundOptions::GetEffectsVolume },
			//{ "SetEffectsVolume", &SoundOptions::SetEffectsVolume },
			//{ "GetVoiceVolume",   &SoundOptions::GetVoiceVolume },
			//{ "SetVoiceVolume",   &SoundOptions::SetVoiceVolume },

			// Steam
			{ "GetSteam64", &Steam::GetSteam64 },

			// Stock Extensions
			{ "DoString", &StockExtensions::DoString },

			// Function register table must end with a zero entry
			{ 0, 0 }
		};

		luaL_register(L, "exu", EXPORT);
		Init(L);

		return 0;
	}
}

#pragma pop_macro("MessageBox")