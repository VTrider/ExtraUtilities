/* Copyright (C) 2023-2026 VTrider
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

#pragma once

#include "BZR.h"
#include "Scanner.h"

#include <cstdint>
#include <memory>

namespace ExtraUtilities::Ogre
{
	struct Fog
	{
		float r;
		float g;
		float b;
		uint8_t padding[4] = { 0 };
		float start;
		float ending;

		Fog() = default;
		Fog(float r, float g, float b, float start, float ending)
			: r(r), g(g), b(b), start(start), ending(ending) {}
	};

	struct Color
	{
		float r, g, b;
	};

	inline Scanner terrain_masterlight(BZR::Ogre::terrain_masterlight, BasicScanner::Restore::DISABLED);
	inline Scanner sceneManager(BZR::Ogre::sceneManagerStructure, { BZR::Ogre::sceneManagerOffset }, BasicScanner::Restore::DISABLED);

	// Gets the fog structure from the scene manager
	inline Fog* GetFog()
	{
		void* sm = sceneManager.Read();
		Fog* fog;
		__asm
		{
			mov eax, [sm]
			add eax, 0x128
			mov [fog], eax
		}
		return fog;
	}

	// Function Pointers

	// This is bordering on voodoo

	using enum BasicScanner::BaseAddress;

	inline uintptr_t getAmbientLightAddr = BasicScanner::CalculateAddress(BZR::Ogre::getAmbientLightOffset, OGRE);
	using _GetAmbientLight = Color*(__thiscall*)(void*);
	inline _GetAmbientLight GetAmbientLight = (_GetAmbientLight)getAmbientLightAddr;

	inline uintptr_t setAmbientLightAddr = BasicScanner::CalculateAddress(BZR::Ogre::setAmbientLightOffset, OGRE);
	using _SetAmbientLight = void(__thiscall*)(void*, Color*);
	inline _SetAmbientLight SetAmbientLight = (_SetAmbientLight)setAmbientLightAddr;

	inline uintptr_t getDiffuseColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::getDiffuseColorOffset, OGRE);
	using _GetDiffuseColor = Color*(__thiscall*)(void*);
	inline _GetDiffuseColor GetDiffuseColor = (_GetDiffuseColor)getDiffuseColorAddr;

	inline uintptr_t setDiffuseColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::setDiffuseColorOffset, OGRE);
	using _SetDiffuseColor = void(__thiscall*)(void*, float, float, float);
	inline _SetDiffuseColor SetDiffuseColor = (_SetDiffuseColor)setDiffuseColorAddr;

	inline uintptr_t getSpecularColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::getSpecularColorOffset, OGRE);
	using _GetSpecularColor = Color*(__thiscall*)(void*);
	inline _GetSpecularColor GetSpecularColor = (_GetDiffuseColor)getSpecularColorAddr;

	inline uintptr_t setSpecularColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpecularColorOffset, OGRE);
	using _SetSpecularColor = void(__thiscall*)(void*, float, float, float);
	inline _SetSpecularColor SetSpecularColor = (_SetSpecularColor)setSpecularColorAddr;

	inline uintptr_t setSpotlightRangeAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpotlightRangeOffset, OGRE);
	using _SetSpotlightRange = void(__thiscall*)(void*, float*, float*, float);
	inline _SetSpotlightRange SetSpotlightRange = (_SetSpotlightRange)setSpotlightRangeAddr;

	inline uintptr_t setVisibleAddr = BasicScanner::CalculateAddress(BZR::Ogre::setVisibleOffset, OGRE);
	using _SetVisible = void(__thiscall*)(void*, bool);
	inline _SetVisible SetVisible = (_SetVisible)setVisibleAddr;
}