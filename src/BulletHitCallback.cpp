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

#include "BulletHitCallback.h"

#include "BZR.h"
#include "Hook.h"
#include "LuaHelpers.h"
#include "LuaState.h"

#include <lua.hpp>

namespace ExtraUtilities::Patch
{
	static void __cdecl LuaCallback(const char* odf, BZR::GameObject* shooter, BZR::GameObject* hitObject, BZR::MAT_3D* transform)
	{
		lua_State* L = Lua::state;

		lua_getglobal(L, "exu");
		lua_getfield(L, -1, "BulletHit");

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
		if (hitObject == nullptr)
		{
			lua_pushnil(L);
		}
		else
		{
			lua_pushlightuserdata(L, reinterpret_cast<void*>(BZR::GameObject::GetHandle(hitObject)));
		}

		// Fourth param
		Lua::PushMatrix(L, *transform);

		lua_call(L, 4, 0);

		lua_pop(L, -1);
	}

	static void __declspec(naked) BulletHitCallback()
	{
		__asm
		{
			// Game code
			mov ecx, [eax + 0x14]
			add ecx, 0x38

			pushad
			pushfd

			// The function calls in this patch appear to modify
			// xmm0, xmm2, and xmm3, we need to save them
			sub esp, 0x10
			movdqu [esp], xmm0

			sub esp, 0x10
			movdqu [esp], xmm2

			sub esp, 0x10
			movdqu [esp], xmm3


			// eax has the this pointer (bullet/ordnance inheritance bs)
			lea ebx, [eax] // I stored it here don't remember why

			lea eax, [ecx-0x18] // matrix
			push eax

			mov eax, [ebp+0x08] // gameobject* of hit object if it exists
			push eax

			// now we're gonna do some voodoo to get the shooter handle
			mov eax, [ebx+0xD8] // obj76 of the ordnance owner

			// this *shouldn't* be null but apparently BL encountered this,
			// needs more testing with modded weapons
			cmp eax, 0x0
			je skip

			mov eax, [eax+0x8C] // gameobject* of the obj76

			skip:

			push eax

			mov eax, [ebx+0x0C] // OrdnanceClass* - Scanner by -0x04 from 1.5, is 0x10 in 1.5
			lea ebx, [eax+0x20] // odf char*
			push ebx

			call LuaCallback
			add esp, 0x10

			movdqu xmm3, [esp]
			add esp, 0x10

			movdqu xmm2, [esp]
			add esp, 0x10

			movdqu xmm0, [esp]
			add esp, 0x10

			popfd
			popad

			ret
		}
	}
	Hook bulletHitHook(bulletHit, &BulletHitCallback, 6, Hook::Status::ACTIVE);
}