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

#include "OrdnanceVelocity.h"

#include "BZR.h"
#include "Hook.h"
#include "LuaHelpers.h"
#include "LuaState.h"

#include <lua.hpp>

namespace ExtraUtilities::Patch
{
	static void __declspec(naked) OrdnanceVelocityPatch()
	{
		__asm
		{
			pushad
			pushfd

			sub esp, 0x10
			movdqu [esp], xmm0

			sub esp, 0x10
			movdqu [esp], xmm1
			
			/*
			 * Notes:
			 * Ordnance* this is at [ebp-0x20]
			 * Ordnance vector3d at x: [ebp-0x10] y: [ebp-0xC] z: [ebp-0x08]
			 * Offset to GameObject velocity: 0x12C
			 */ 

			// Get owner of the ordnance
			mov ecx, [ebp-0x20]
			mov eax, [ecx+0xD8] // obj76
			mov eax, [eax+0x8C] // GameObject* owner

			movdqu xmm0, [eax+0x12C] // load shooter velocity

			movdqu xmm1, [ebp-0x10] // load ordnance velocity

			addps xmm0, xmm1 // inherit velocity

			// unpack register and replace values
			movss [ebp-0x10], xmm0
			pextrd eax, xmm0, 1
			mov [ebp-0xC], eax
			pextrd eax, xmm0, 2
			mov [ebp-0x08], eax

			movdqu xmm1, [esp]
			add esp, 0x10

			movdqu xmm0, [esp]
			add esp, 0x10

			popfd
			popad

			// Game code
			mov ecx, [ebp-0x10]
			mov [eax], ecx
			mov edx, [ebp-0xC]

			ret
		}
	}
	Hook ordnanceVelocityPatch(ordnanceVelocity, &OrdnanceVelocityPatch, 8, BasicPatch::Status::ACTIVE);
}