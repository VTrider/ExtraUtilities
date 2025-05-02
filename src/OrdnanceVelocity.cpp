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
#include "InlinePatch.h"
#include "Hook.h"
#include "LuaHelpers.h"
#include "LuaState.h"

#include <lua.hpp>

namespace ExtraUtilities::Patch
{
	static float __cdecl DotProduct(BZR::VECTOR_3D* v, BZR::VECTOR_3D* w)
	{
		return v->x * w->x + v->y * w->y + v->z * w->z;
	}

	static void __declspec(naked) OrdnanceVelocityPatch()
	{
		__asm
		{
			/*
			 * Notes:
			 * Ordnance* this is at [ebp-0x20]
			 * Ordnance vector3d at x: [ebp-0x10] y: [ebp-0xC] z: [ebp-0x08]
			 * Offset to GameObject velocity: 0x12C
			 * Offset to obj76 front vector 0x38
			 */

			push ebp
			mov ebp, esp

			sub esp, 0x04 // allocate a local for the dot product result

			// Save registers
			pushad
			pushfd

			sub esp, 0x10
			movdqu [esp], xmm0

			sub esp, 0x10
			movdqu [esp], xmm1

			sub esp, 0x10
			movdqu [esp], xmm2
			
			// Begin patch

			mov edi, [ebp]

			// Get owner of the ordnance
			mov ecx, [edi-0x20]
			mov eax, [ecx+0xD8] // obj76
			mov ebx, [eax+0x8C] // GameObject* owner

			movups xmm0, [ebx+0x12C]

			cmp [velocOnlyInheritFront], 0x1
			jne inheritAll

			push eax // save this from the call

			// Dot the front vector and velocity vector
			lea edx, [eax+0x38] // shooter front
			push edx
			lea edx, [ebx+0x12C] // shooter velocity
			push edx
			call DotProduct
			add esp, 0x8

			pop eax

			fstp [ebp-0x04] // dot result

			movups xmm0, [eax+0x38] // shooter front
			movss xmm1, [ebp-0x04]
			shufps xmm1, xmm1, 0 // pack with singles
			mulps xmm0, xmm1 // scale by front velocity

			// Ignore the Y value
			movups xmm2, [velocIgnoreY]
			mulps xmm0, xmm2

			inheritAll:

			movdqu xmm1, [edi-0x10] // load ordnance velocity

			movss xmm2, [velocInheritRatio]
			shufps xmm2, xmm2, 0 // pack singles
			mulps xmm1, xmm2 // apply user defined scaling factor

			addps xmm0, xmm1 // inherit velocity

			// unpack register and replace values
			movss [edi-0x10], xmm0
			pextrd eax, xmm0, 1
			mov [edi-0xC], eax
			pextrd eax, xmm0, 2
			mov [edi-0x08], eax

			// End patch

			// Restore registers
			movdqu xmm2, [esp]
			add esp, 0x10

			movdqu xmm1, [esp]
			add esp, 0x10

			movdqu xmm0, [esp]
			add esp, 0x10

			popfd
			popad

			mov esp, ebp
			pop ebp

			// Game code
			mov ecx, [ebp-0x10]
			mov [eax], ecx
			mov edx, [ebp-0xC]

			ret
		}
	}
	Hook ordnanceVelocityPatch(ordnanceVelocity, &OrdnanceVelocityPatch, 8, BasicPatch::Status::INACTIVE);

	static void __declspec(naked) CannonLeadPositionPatch()
	{
		__asm
		{
			/*
			* Notes:
			* eax has target velocity
			* [ebp-0x20] has the this pointer (cannon)
			* 
			* need to subtract shooter velocity from the 
			* target velocity to fix the TLI position
			*/

			push ebp
			mov ebp, esp

			sub esp, 0x04 // dot result

			pushad
			pushfd

			sub esp, 0x10
			movdqu [esp], xmm0

			sub esp, 0x10
			movdqu [esp], xmm1

			sub esp, 0x10
			movdqu [esp], xmm2

			// Begin patch

			mov edi, [ebp] // old stack frame

			mov ecx, [edi-0x20] // this pointer (cannon)
			mov ebx, [ecx+0x18] // cannon owner obj76
			mov edx, [ebx+0x8C] // obj76 owner (GameObject*)

			push eax // this has target velocity
			movups xmm1, [edx + 0x12C] // shooter velocity

			cmp [velocOnlyInheritFront], 0x1
			jne inheritAll

			push ebx // save this from the call

			// Dot the front vector and velocity vector
			lea esi, [ebx + 0x38] // shooter front
			push esi
			lea esi, [edx + 0x12C] // shooter velocity
			push esi
			call DotProduct
			add esp, 0x8

			pop ebx

			fstp [ebp-0x04]

			movups xmm1, [ebx + 0x38] // shooter front
			movss xmm2, [ebp-0x04]
			shufps xmm2, xmm2, 0 // pack with singles
			mulps xmm1, xmm2 // scale by front velocity

			// Ignore the Y value
			movups xmm2, [velocIgnoreY]
			mulps xmm1, xmm2

			inheritAll:

			pop eax
			movups xmm0, [eax]

			subps xmm0, xmm1 // target velocity - shooter velocity = the value we want for the TLI calculation

			// unpack values and replace
			movss [edi-0x1C], xmm0
			pextrd ebx, xmm0, 1
			mov [edi-0x18], ebx
			pextrd ebx, xmm0, 2
			mov [edi-0x14], ebx

			// End patch

			movdqu xmm2, [esp]
			add esp, 0x10

			movdqu xmm1, [esp]
			add esp, 0x10

			movdqu xmm0, [esp]
			add esp, 0x10

			popfd
			popad

			mov esp, ebp
			pop ebp

			// Game code
			mov eax, [ebp-0x20]
			mov ecx, [eax+0x0C]

			ret
		}
	}
	Hook cannonLeadPositionPatch(cannonLeadPosition, &CannonLeadPositionPatch, 6, BasicPatch::Status::INACTIVE);

	void __declspec(naked)MortarLeadPositionPatch()
	{
		__asm
		{

		}
	}

	// This bypasses the is target speed > 0.1 m/s check, you still want to calculate
	// lead position even on still targets because it will be affected by the shooter's speed
	InlinePatch cannonVelocityTolerancePatch(cannonVelocityTolerance, BasicPatch::NOP, 6, BasicPatch::Status::INACTIVE);
}

namespace ExtraUtilities::Lua::Patches
{
	int GetOrdnanceVelocInheritance(lua_State* L)
	{
		// This is only one component but they are all activated in sync
		lua_pushboolean(L, Patch::ordnanceVelocityPatch.IsActive());
		return 1;
	}

	int SetOrdnanceVelocInheritance(lua_State* L)
	{
		bool active = CheckBool(L, 1);
		if (active == true)
		{
			Patch::ordnanceVelocityPatch.Reload();
			Patch::cannonLeadPositionPatch.Reload();
			Patch::cannonVelocityTolerancePatch.Reload();
		}
		else
		{
			Patch::ordnanceVelocityPatch.Unload();
			Patch::cannonLeadPositionPatch.Unload();
			Patch::cannonVelocityTolerancePatch.Unload();
		}
		return 0;
	}
}