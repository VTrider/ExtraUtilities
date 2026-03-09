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
#include <string>

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

	inline uintptr_t getDirectionAddr = BasicScanner::CalculateAddress(0x14042, OGRE);
	using _GetDirection = BZR::VECTOR_3D*(__thiscall*)(void*);
	inline _GetDirection GetDirection = (_GetDirection)getDirectionAddr;

	inline uintptr_t setDirectionAddr = BasicScanner::CalculateAddress(0x0FB69, OGRE);
	using _SetDirection = void(__thiscall*)(void*, float, float, float);
	inline _SetDirection SetDirection = (_SetDirection)setDirectionAddr;

	inline uintptr_t getPowerScaleAddr = BasicScanner::CalculateAddress(0x3A5CB, OGRE);
	using _GetPowerScale = float(__thiscall*)(void*);
	inline _GetPowerScale GetPowerScale = (_GetPowerScale)getPowerScaleAddr;

	inline uintptr_t setPowerScaleAddr = BasicScanner::CalculateAddress(0x10FBE, OGRE);
	using _SetPowerScale = void(__thiscall*)(void*, float);
	inline _SetPowerScale SetPowerScale = (_SetPowerScale)setPowerScaleAddr;

	inline uintptr_t getShadowFarDistanceAddr = BasicScanner::CalculateAddress(0x1A9B0, OGRE);
	using _GetShadowFarDistance = float(__thiscall*)(void*);
	inline _GetShadowFarDistance GetShadowFarDistance = (_GetShadowFarDistance)getShadowFarDistanceAddr;

	inline uintptr_t setShadowFarDistanceAddr = BasicScanner::CalculateAddress(0x0F5C4, OGRE);
	using _SetShadowFarDistance = void(__thiscall*)(void*, float);
	inline _SetShadowFarDistance SetShadowFarDistance = (_SetShadowFarDistance)setShadowFarDistanceAddr;

	inline uintptr_t setSpotlightRangeAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpotlightRangeOffset, OGRE);
	using _SetSpotlightRange = void(__thiscall*)(void*, float*, float*, float);
	inline _SetSpotlightRange SetSpotlightRange = (_SetSpotlightRange)setSpotlightRangeAddr;

	inline uintptr_t setVisibleAddr = BasicScanner::CalculateAddress(BZR::Ogre::setVisibleOffset, OGRE);
	using _SetVisible = void(__thiscall*)(void*, bool);
	inline _SetVisible SetVisible = (_SetVisible)setVisibleAddr;

	inline uintptr_t getVisibleAddr = BasicScanner::CalculateAddress(0x05E70, OGRE);
	using _GetVisible = bool(__thiscall*)(void*);
	inline _GetVisible GetVisible = (_GetVisible)getVisibleAddr;

	inline uintptr_t getCastShadowsAddr = BasicScanner::CalculateAddress(0x37C40, OGRE);
	using _GetCastShadows = bool(__thiscall*)(void*);
	inline _GetCastShadows GetCastShadows = (_GetCastShadows)getCastShadowsAddr;

	inline uintptr_t setCastShadowsAddr = BasicScanner::CalculateAddress(0x325C4, OGRE);
	using _SetCastShadows = void(__thiscall*)(void*, bool);
	inline _SetCastShadows SetCastShadows = (_SetCastShadows)setCastShadowsAddr;

	inline uintptr_t getNumSubEntitiesAddr = BasicScanner::CalculateAddress(0x2635F, OGRE);
	using _GetNumSubEntities = uint32_t(__thiscall*)(void*);
	inline _GetNumSubEntities GetNumSubEntities = (_GetNumSubEntities)getNumSubEntitiesAddr;

	inline uintptr_t getSubEntityByIndexAddr = BasicScanner::CalculateAddress(0x1A163, OGRE);
	using _GetSubEntityByIndex = void*(__thiscall*)(void*, uint32_t);
	inline _GetSubEntityByIndex GetSubEntityByIndex = (_GetSubEntityByIndex)getSubEntityByIndexAddr;

	inline uintptr_t getMaterialNameSubEntityAddr = BasicScanner::CalculateAddress(0x07B08, OGRE);
	using _GetMaterialNameSubEntity = const std::string&(__thiscall*)(void*);
	inline _GetMaterialNameSubEntity GetMaterialNameSubEntity = (_GetMaterialNameSubEntity)getMaterialNameSubEntityAddr;

	inline uintptr_t setMaterialNameEntityAddr = BasicScanner::CalculateAddress(0x1BF9F, OGRE);
	using _SetMaterialNameEntity = void(__thiscall*)(void*, const std::string&, const std::string&);
	inline _SetMaterialNameEntity SetMaterialNameEntity = (_SetMaterialNameEntity)setMaterialNameEntityAddr;

	inline uintptr_t setMaterialNameSubEntityAddr = BasicScanner::CalculateAddress(0x06E3D, OGRE);
	using _SetMaterialNameSubEntity = void(__thiscall*)(void*, const std::string&, const std::string&);
	inline _SetMaterialNameSubEntity SetMaterialNameSubEntity = (_SetMaterialNameSubEntity)setMaterialNameSubEntityAddr;
}
