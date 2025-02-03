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

#include "BulletHit.h"

#include "BZR.h"
#include "Hook.h"
#include "LuaState.h"

#include <lua.hpp>

namespace ExtraUtilities::Patch
{
	static void __cdecl LuaCallback(const char* odf, int* obj, double position[3])
	{
		lua_State* L = Lua::state;

		lua_getglobal(L, "SetVector");
		lua_pushnumber(L, position[0]);
		lua_pushnumber(L, position[1]);
		lua_pushnumber(L, position[2]);
		lua_call(L, 3, 1);
		int posVector = lua_gettop(L);

		lua_getglobal(L, "exu");
		lua_getfield(L, -1, "BulletHit");

		if (!lua_isfunction(L, -1))
		{
			return;
		}

		lua_pushstring(L, odf);
		if (*obj == 0)
		{
			lua_pushnil(L);
		}
		else
		{
			lua_pushlightuserdata(L, reinterpret_cast<void*>(BZR::GameObject::GetHandle(*obj)));
		}
		lua_pushvalue(L, posVector);
		lua_call(L, 3, 0);
		lua_pop(L, -1);
	}

	static __declspec(naked) void BulletHitCallback()
	{
		// stuff like this MUST be static if you wanna keep it in the scope of the function
		// cause it will be stored elsewhere and you want the hook to only use the asm
		static uintptr_t jmpBack = bulletHit + 7;
		// I wrote this a long time ago so idk wtf is going on
		__asm
		{
			pushad
			pushfd

			// eax has the this pointer (bullet/ordnance inheritance bs)
			lea ebx, [eax] // I stored it here don't remember why

			lea eax, [ecx + 0x48] // position, vec3 of doubles 8*3 bytes
			push eax

			lea eax, [ebp + 0x08] // gameobject* of hit object if it exists
			push eax

			mov eax, [ebx + 0x0C] // OrdnanceClass* - offset by -0x04 from 1.5, is 0x10 in 1.5
			lea ebx, [eax + 0x20] // odf char*
			push ebx

			call LuaCallback
			add esp, 0x0C

			popfd
			popad

			add ecx, 0x38
			push ecx
			mov edx, [ebp + 0x0C]

			jmp [jmpBack]
		}
	}
	Hook bulletHitHook(bulletHit, &BulletHitCallback, 7);
}