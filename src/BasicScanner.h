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

// Non-template properties and helpers for the scanner class

#pragma once

#include <cstdint>

#include <Windows.h>

#undef ABSOLUTE

namespace ExtraUtilities
{
	class BasicScanner
	{
	protected:
		static inline uintptr_t m_bzrModuleBase = reinterpret_cast<uintptr_t>(GetModuleHandle("Battlezone98Redux.exe"));
		static inline uintptr_t m_ogreMainModuleBase = reinterpret_cast<uintptr_t>(GetModuleHandle("OgreMain.dll"));

		BasicScanner() = default;
		virtual ~BasicScanner() = default;

	public:
		// Should the scanner restore the original data when the dll exits?
		enum class Restore : uint8_t
		{
			ENABLED,
			DISABLED
		};

		// What base address to use when calculating memory addresses
		enum class BaseAddress : uint8_t
		{
			ABSOLUTE, // use absolute address, for most cases in BZR
			BZR, // use BZR module base, not necessary due to lack of ASLR
			OGRE // use ogre module base, necessary for ogre functions since the dll is loaded in a random location
		};

		// Calculates an address from module offset for use in a hook as opposed to accessing data
		static uintptr_t CalculateAddress(uintptr_t offset, BaseAddress baseAddress)
		{
			using enum BaseAddress;
			switch (baseAddress)
			{
			case ABSOLUTE:
				return offset;
			case BZR:
				return m_bzrModuleBase + offset;
			case OGRE:
				return m_ogreMainModuleBase + offset;
			default:
				return 0;
			}
		}
	};
}
