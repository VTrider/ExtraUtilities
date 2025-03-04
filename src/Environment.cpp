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

#include "Environment.h"

#include "Hook.h"
#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::Environment
{
	int GetGravity(lua_State* L)
	{
		BZR::VECTOR_3D g = gravity.Read();

		PushVector(L, g);

		return 1;
	}

	// You can use individual floats or a vector 3d
	int SetGravity(lua_State* L)
	{
		auto newGravity = CheckVectorOrSingles(L, 1);
		gravity.Write(newGravity);

		return 0;
	}

	int GetFog(lua_State* L)
	{
		Ogre::Fog f = fog.Read();

		lua_createtable(L, 0, 5);

		lua_pushnumber(L, f.r);
		lua_setfield(L, -2, "r");

		lua_pushnumber(L, f.g);
		lua_setfield(L, -2, "g");

		lua_pushnumber(L, f.b);
		lua_setfield(L, -2, "b");

		lua_pushnumber(L, f.start);
		lua_setfield(L, -2, "start");

		lua_pushnumber(L, f.ending);
		lua_setfield(L, -2, "ending");

		return 1;
	}

	int SetFog(lua_State* L)
	{
		float r = static_cast<float>(luaL_checknumber(L, 1));
		float g = static_cast<float>(luaL_checknumber(L, 2));
		float b = static_cast<float>(luaL_checknumber(L, 3));
		float start = static_cast<float>(luaL_checknumber(L, 4));
		float ending = static_cast<float>(luaL_checknumber(L, 5));

		fog.Write({ r, g, b, start, ending });

		return 0;
	}

	int GetSunAmbient(lua_State* L)
	{
		auto* sunAmbient = Ogre::GetAmbientLight(Ogre::sceneManager.Read());

		PushColor(L, *sunAmbient);

		return 1;
	}

	int SetSunAmbient(lua_State* L)
	{
		auto color = CheckColorOrSingles(L, 1);

		Ogre::SetAmbientLight(Ogre::sceneManager.Read(), &color);
		// this value is what we tell the game to read when it tries to reset the
		// sunlight parameters, so we need to set it as well
		desiredSunAmbient = color;

		return 0;
	}

	int GetSunDiffuse(lua_State* L)
	{
		auto* color = Ogre::GetDiffuseColor(Ogre::terrain_masterlight.Read());

		PushColor(L, *color);

		return 1;
	}

	int SetSunDiffuse(lua_State* L)
	{
		auto color = CheckColorOrSingles(L, 1);

		Ogre::SetDiffuseColor(Ogre::terrain_masterlight.Read(), color.r, color.g, color.b);
		desiredSunDiffuse = color;

		return 0;
	}
}

namespace ExtraUtilities::Patch
{
	static __declspec(naked) void SunAmbientResetPatch()
	{
		__asm
		{
			// Game code
			fstp [ebp-0x0C]
			
			lea eax, [Lua::Environment::desiredSunAmbient]

			ret
		}
	}
	Hook sunAmbientResetPatch(sunAmbientReset, &SunAmbientResetPatch, 6, BasicPatch::Status::ACTIVE);

	static __declspec(naked) void SunDiffuseResetPatch()
	{
		__asm
		{
			// Game code
			fstp[ebp - 0x0C]

			lea edx, [Lua::Environment::desiredSunDiffuse]

			ret
		}
	}
	Hook sunDiffuseResetPatch(sunDiffuseReset, &SunDiffuseResetPatch, 6, BasicPatch::Status::ACTIVE);

	static __declspec(naked) void SunSpecularResetPatch()
	{
		__asm
		{
			// Game code
			fstp[ebp - 0x0C]

			lea eax, [Lua::Environment::desiredSunSpecular]

			ret
		}
	}
	Hook sunSpecularResetPatch(sunSpecularReset, &SunSpecularResetPatch, 6, BasicPatch::Status::ACTIVE);
}