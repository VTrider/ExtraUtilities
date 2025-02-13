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

	inline uintptr_t setDiffuseColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::setDiffuseColorOffset, BasicScanner::BaseAddress::OGRE);
	using _SetDiffuseColor = void(__thiscall*)(void*, float, float, float);
	inline _SetDiffuseColor SetDiffuseColor = (_SetDiffuseColor)setDiffuseColorAddr;

	inline uintptr_t setSpecularColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpecularColorOffset, BasicScanner::BaseAddress::OGRE);
	using _SetSpecularColor = void(__thiscall*)(void*, float, float, float);
	inline _SetSpecularColor SetSpecularColor = (_SetSpecularColor)setSpecularColorAddr;

	inline uintptr_t setSpotlightRangeAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpotlightRangeOffset, BasicScanner::BaseAddress::OGRE);
	using _SetSpotlightRange = void(__thiscall*)(void*, float*, float*, float);
	inline _SetSpotlightRange SetSpotlightRange = (_SetSpotlightRange)setSpotlightRangeAddr;

	inline uintptr_t setVisibleAddr = BasicScanner::CalculateAddress(BZR::Ogre::setVisibleOffset, BasicScanner::BaseAddress::OGRE);
	using _SetVisible = void(__thiscall*)(void*, bool);
	inline _SetVisible SetVisible = (_SetVisible)setVisibleAddr;
}

namespace ExtraUtilities::Patch
{
	inline const uintptr_t sceneManagerHookAddr = BasicScanner::CalculateAddress(0x3B521E, BasicScanner::BaseAddress::OGRE);

	inline std::unique_ptr<Scanner<Ogre::Fog>> fog;
	inline std::unique_ptr<Scanner<Ogre::Color>> sunAmbient;
}