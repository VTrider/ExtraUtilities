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

#include "Scanner.h"

#include <cstdint>
#include <memory>

namespace ExtraUtilities
{
	struct OgreFog
	{
		OgreFog() = default;
		OgreFog(float r, float g, float b, float start, float ending)
			: r(r), g(g), b(b), start(start), ending(ending) {}

		float r;
		float g;
		float b;
		uint8_t padding[4] = {0};
		float start;
		float ending;
	};

	struct OgreColor
	{
		float r, g, b;
	};
}

namespace ExtraUtilities::Patch
{
	inline const uintptr_t sceneManagerHookAddr = BasicScanner::CalculateAddress(0x3B521E, BasicScanner::BaseAddress::OGRE);

	inline std::unique_ptr<Scanner<OgreFog>> fog;
	inline std::unique_ptr<Scanner<OgreColor>> sunAmbient;
}