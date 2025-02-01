/* Copyright (C) 2023-2024 VTrider
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

/*----------------------------------
* Exported lua functions go here,  *
* sends/receieves data to/from the *
* lua stack						   *
-----------------------------------*/

#include "About.h"
#include "Exports.h"
#include "LuaState.h"

#include "lua.hpp"
#include <Windows.h>

namespace ExtraUtilities::Lua
{
	void Init(lua_State* L)
	{
		state = L;

		lua_getglobal(L, "exu");
		lua_pushstring(L, version.c_str());
		lua_setfield(L, -2, "version");

		lua_pop(L, -1);
	}

	extern "C"
	{
		int __declspec(dllexport) luaopen_exu(lua_State* L)
		{
			const luaL_Reg EXPORT[] = {
				// Environment
				{ "GetGravity", &Environment::GetGravity },
				{ "SetGravity", &Environment::SetGravity },

				// Reticle
				{ "GetReticlePos", &Reticle::GetPosition },
				{ "GetReticleRange", &Reticle::GetRange },
				{ "SetReticleRange", &Reticle::SetRange },
				{ "GetReticleObject", &Reticle::GetObject},
				{ "GetReticleMatrix", &Reticle::GetMatrix},
				{ 0, 0 }
			};
			luaL_register(L, "exu", EXPORT);

			Init(L);

			return 0;
		}
	}
}