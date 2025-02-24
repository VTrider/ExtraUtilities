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

#include "BulletInitCallback.h"

#include "BZR.h"
#include "Hook.h"
#include "LuaHelpers.h"
#include "LuaState.h"

namespace ExtraUtilities::Patch
{
	static void __cdecl LuaCallback(const char* odf, BZR::GameObject* shooter, BZR::MAT_3D* transform)
	{
		lua_State* L = Lua::state;

		lua_getglobal(L, "exu");
		lua_getfield(L, -1, "BulletInit");

		if (!lua_isfunction(L, -1))
		{
			return;
		}

		int len = strlen(odf);
		char formattedODF[16]; // this should be enough room given the 8 char limit

		strncpy(formattedODF, odf, len - 4); // strip the ".odf" from the name

		lua_pushlstring(L, formattedODF, len - 4); // First param

		// Second param
		if (shooter == nullptr)
		{
			lua_pushnil(L);
		}
		else
		{
			lua_pushlightuserdata(L, reinterpret_cast<void*>(BZR::GameObject::GetHandle(shooter)));
		}

		// Third param
		Lua::PushMatrix(L, *transform);

		lua_call(L, 3, 0);

		lua_pop(L, -1);
	}

	static void __declspec(naked) BulletInitCallback()
	{
		__asm
		{
			 pushad
			 pushfd

			 mov ecx, [ebp-0x20] // bullet* this
			 mov eax, [ecx+0xD8] // obj76

			 mov ebx, [ebp+0x08] // MAT_3D transform
			 push ebx // third param

			 mov ebx, [eax+0x8C] // GameObject* shooter
			 push ebx // second param

			 mov ebx, [ecx+0xC] // OrdnanceClass*
			 lea ebx, [ebx+0x20] // odf char*
			 push ebx // first param

			 call LuaCallback
			 add esp, 0xC

			 popfd
			 popad

			// game code
			mov edx, [ebp-0x20]
			mov eax, [edx+0x14]

			ret
		}
	}
	Hook bulletInitCallback(bulletInit, &BulletInitCallback, 6, BasicPatch::Status::ACTIVE);
}