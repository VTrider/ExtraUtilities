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

#include "Ogre.h"

#include "Hook.h"

namespace ExtraUtilities::Patch
{
	void __cdecl InitOgreScanners(void* sceneManager)
	{
		Ogre::Fog* fogOffset;
		Ogre::Color* sunlightOffset;

		__asm
		{
			mov eax, [sceneManager]
			add eax, 0x28
			mov [sunlightOffset], eax

			mov eax, [sceneManager]
			add eax, 0x128
			mov [fogOffset], eax
		}

		// these gotta be heap allocated cause we can't reference local vars
		fog = std::make_unique<Scanner<Ogre::Fog>>(fogOffset);
		sunAmbient = std::make_unique<Scanner<Ogre::Color>>(sunlightOffset);
	}

	static void __declspec(naked) GetOgreSceneManager()
	{	
		static int doneInit = 0x0;
		__asm
		{
			// we need to grab the scene manager once and
			// initialize the fog and sunlight scanners

			cmp [doneInit], 0x0
			jnz resume

			// this is fked up but you need to push esp because the call and shit
			// will modify it so when you popad (necessary cause who knows what registers the stl
			// functions use) it will restore the WRONG esp value and crash!
			push esp
			pushad

			push ecx
			call InitOgreScanners
			add esp, 0x4
			mov [doneInit], 0x1

			popad
			pop esp

			resume:

			// Game code
			mov ebx, ecx
			mov eax, [ebx]
			mov edi, [ebp+0x08]

			ret
		}
	}
	Hook sceneManagerHook(sceneManagerHookAddr, &GetOgreSceneManager, 7, Hook::Status::ACTIVE);
}