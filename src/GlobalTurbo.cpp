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

#include "GlobalTurbo.h"

#include "BZR.h"
#include "Hook.h"
#include "InlinePatch.h"
#include "LuaHelpers.h"

namespace ExtraUtilities::Patch
{
	InlinePatch turboPatch1(comissPatch, &patchedTurboTolerance, InlinePatch::Status::INACTIVE);
	InlinePatch turboPatch2(turboConditionPatch, BasicPatch::NOP, 2, InlinePatch::Status::INACTIVE);

	enum class TurboCode
	{
		BEGIN = 0,
		END = 1
	};

	static void __cdecl DoSelectiveTurboPatch(BZR::GameObject* obj, TurboCode code)
	{
		BZR::handle h = BZR::GameObject::GetHandle(obj);
		switch (code)
		{
		case TurboCode::BEGIN:
			if (setTurboUnits.contains(h))
			{
				turboPatch1.SetStatus(setTurboUnits.at(h));
				turboPatch2.SetStatus(setTurboUnits.at(h));
			}
			break;
		case TurboCode::END:
			if (setTurboUnits.contains(h))
			{
				turboPatch1.SetStatus(globalTurboEnabled);
				turboPatch2.SetStatus(globalTurboEnabled);
			}
			break;
		}
	}

	static void __declspec(naked) TurboPatchBegin()
	{
		__asm
		{
			// Notes:
			// ecx has the unit task, ecx+0x10 is the "me" gameobject*

			pushad
			pushfd

			push 0x0
			mov eax, [eax+0x10]
			push eax
			call DoSelectiveTurboPatch
			add esp, 0x08

			popfd
			popad

			// Game code
			mov eax, [ebp-0x70]
			fstp [eax+0x08]

			ret
		}
	}
	Hook turboPatchBegin(turboPatchBeginAddr, &TurboPatchBegin, 6, InlinePatch::Status::ACTIVE);

	static void __declspec(naked) TurboPatchEnd()
	{
		__asm
		{
			pushad
			pushfd

			push 0x1
			mov eax, [edx+0x10]
			push eax
			call DoSelectiveTurboPatch
			add esp, 0x08

			popfd
			popad

			// Game code
			mov edx, [ebp-0x70]
			mov eax, [ebp-0x88]

			ret
		}
	}
	Hook turboPatchEnd(turboPatchEndAddr, &TurboPatchEnd, 9, InlinePatch::Status::ACTIVE);
}

namespace ExtraUtilities::Lua::Patches
{
	int GetGlobalTurbo(lua_State* L)
	{
		if (Patch::turboPatch1.IsActive() && Patch::turboPatch2.IsActive())
		{
			lua_pushboolean(L, true);
		}
		else
		{
			lua_pushboolean(L, false);
		}

		return 0;
	}

	int SetGlobalTurbo(lua_State* L)
	{
		bool status = CheckBool(L, 1);

		if (status == true)
		{
			Patch::turboPatch1.Reload();
			Patch::turboPatch2.Reload();
		}
		else
		{
			Patch::turboPatch1.Unload();
			Patch::turboPatch2.Unload();
		}

		return 0;
	}

	int GetUnitTurbo(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);

		bool result;
		if (Patch::setTurboUnits.contains(h))
		{
			result = Patch::setTurboUnits.at(h);
		}
		else
		{
			result = false;
		}

		lua_pushboolean(L, result);

		return 1;
	}

	int SetUnitTurbo(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		bool status = CheckBool(L, 2);

		Patch::setTurboUnits[h] = status;

		return 0;
	}
}