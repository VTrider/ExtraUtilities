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

#include "Memory.h"

#include "Offsets.h"

#include <iostream>
#include <format>

// this file is really just to define the ogre functions globally at runtime,
// everything else is in the header

std::uintptr_t setDiffuseColour{};
std::uintptr_t setSpecularColour{};
std::uintptr_t setSpotlightRange{};
std::uintptr_t fogHook;

void Memory::DefineOgreFunctions()
{
	setDiffuseColour = GetOgreFunction("?setDiffuseColour@Light@Ogre@@QAEXMMM@Z");
	setSpecularColour = GetOgreFunction("?setSpecularColour@Light@Ogre@@QAEXMMM@Z");
	setSpotlightRange = GetOgreFunction("?setSpotlightRange@Light@Ogre@@QAEXABVRadian@2@0M@Z");
	fogHook = reinterpret_cast<DWORD>(Memory::ogreMain) + 0x3B5601;
}

std::uintptr_t Misc::playOption = Memory::GetPointerAddress(0x0094672C, { 0x30 });
