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

#pragma push_macro("MessageBox")
#undef MessageBox

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
				// Control Panel
				{ "SelectAdd",  ControlPanel::SelectAdd },
				{ "SelectNone", ControlPanel::SelectNone },
				{ "SelectOne",  ControlPanel::SelectOne },

				// Environment
				{ "GetGravity", &Environment::GetGravity },
				{ "SetGravity", &Environment::SetGravity },

				// GameObject
				{ "GetHandle", GameObject::GetHandle },
				{ "GetObj",    GameObject::GetObj },
				{ "SetAsUser", GameObject::SetAsUser },

				// OS
				{ "MessageBox",			 &OS::MessageBox },
				{ "GetScreenResolution", &OS::GetScreenResolution },

				// Patches
				{ "GetShotConvergence", &Patches::GetShotConvergence },
				{ "SetShotConvergence", &Patches::SetShotConvergence },

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
				
				{ 0, 0 }
			};
			luaL_register(L, "exu", EXPORT);

			Init(L);

			return 0;
		}
	}
}

#pragma pop_macro("MessageBox")