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

#include "Environment.h"

#include "Hook.h"
#include "InlinePatch.h"
#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::Environment
{
	int GetGravity(lua_State* L)
	{
		BZR::VECTOR_3D g = gravity.Read();

		PushVector(L, g);

		return 1;
	}

	int SetGravity(lua_State* L)
	{
		auto newGravity = CheckVectorOrSingles(L, 1);
		gravity.Write(newGravity);

		return 0;
	}

	int GetFog(lua_State* L)
	{
		Patch::TryInitializeOgre();

		Ogre::Fog f = fog.Read();

		PushFog(L, f);

		return 1;
	}

	int SetFog(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto f = CheckFogOrSingles(L, 1);

		fog.Write(f);

		return 0;
	}

	int GetSunAmbient(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sunAmbient = Ogre::GetAmbientLight(Ogre::sceneManager.Read());

		PushColor(L, *sunAmbient);

		return 1;
	}

	int SetSunAmbient(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto color = CheckColorOrSingles(L, 1);

		Ogre::SetAmbientLight(Ogre::sceneManager.Read(), &color);
		// this value is what we tell the game to read when it tries to reset the
		// sunlight parameters, so we need to set it as well
		Patch::desiredSunAmbient = color;

		return 0;
	}

	int GetSunDiffuse(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* color = Ogre::GetDiffuseColor(Ogre::terrain_masterlight.Read());

		PushColor(L, *color);

		return 1;
	}

	int SetSunDiffuse(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto color = CheckColorOrSingles(L, 1);

		Ogre::SetDiffuseColor(Ogre::terrain_masterlight.Read(), color.r, color.g, color.b);
		Patch::desiredSunDiffuse = color;

		return 0;
	}

	int GetSunSpecular(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* color = Ogre::GetSpecularColor(Ogre::terrain_masterlight.Read());

		PushColor(L, *color);

		return 1;
	}

	int SetSunSpecular(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto color = CheckColorOrSingles(L, 1);

		Ogre::SetSpecularColor(Ogre::terrain_masterlight.Read(), color.r, color.g, color.b);
		Patch::desiredSunSpecular = color;

		return 0;
	}
}

namespace ExtraUtilities::Patch
{
	// Prevents fog reset function from running, the same method doesn't work for the sun for some reason
	InlinePatch fogResetPatch(fogReset, BasicPatch::RET, BasicPatch::Status::INACTIVE);

	// Sunlight reset patches
	static __declspec(naked) void SunAmbientResetPatch()
	{
		__asm
		{
			// Game code
			fstp [ebp-0x0C]
			
			lea eax, [Patch::desiredSunAmbient]

			ret
		}
	}
	Hook sunAmbientResetPatch(sunAmbientReset, &SunAmbientResetPatch, 6, BasicPatch::Status::INACTIVE);

	static __declspec(naked) void SunDiffuseResetPatch()
	{
		__asm
		{
			// Game code
			fstp[ebp - 0x0C]

			lea edx, [Patch::desiredSunDiffuse]

			ret
		}
	}
	Hook sunDiffuseResetPatch(sunDiffuseReset, &SunDiffuseResetPatch, 6, BasicPatch::Status::INACTIVE);

	static __declspec(naked) void SunSpecularResetPatch()
	{
		__asm
		{
			// Game code
			fstp[ebp - 0x0C]

			lea eax, [Patch::desiredSunSpecular]

			ret
		}
	}
	Hook sunSpecularResetPatch(sunSpecularReset, &SunSpecularResetPatch, 6, BasicPatch::Status::INACTIVE);

	/*
	* This waits to initialize the ogre hooks until you call an ogre function
	* in order to prevent the game crashing when alt tabbed in the loading screen
	*/
	void TryInitializeOgre()
	{
		static bool done = false;
		if (!done)
		{
			desiredSunAmbient = *Ogre::GetAmbientLight(Ogre::sceneManager.Read());
			desiredSunDiffuse = *Ogre::GetDiffuseColor(Ogre::terrain_masterlight.Read());
			desiredSunSpecular = *Ogre::GetSpecularColor(Ogre::terrain_masterlight.Read());
			fogResetPatch.Reload();
			sunAmbientResetPatch.Reload();
			sunDiffuseResetPatch.Reload();
			sunSpecularResetPatch.Reload();
			done = true;
		}
	}
}