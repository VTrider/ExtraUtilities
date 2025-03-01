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

/*
* Memory Scanner class with automatic
* protection and restoration of the original
* data
*/

#pragma once

#include "BasicScanner.h"

#include <Windows.h>

#include <vector>

#undef ABSOLUTE // win32 name collision

namespace ExtraUtilities
{
	template <class T>
	class Scanner : public BasicScanner
	{
	private:
		// This needs to be initialized first so that the address of the
		// desired value can be resolved properly
		BaseAddress m_baseAddress; // what base address to use when accessing memory

		T* m_address;
		Restore m_restoreData; // should you restore the data after leaving game? usually yes but sometimes no for things like game settings
		T m_originalData;

		DWORD m_oldProtect{}; // Original memory protection value
		static inline DWORD m_dummyProtect; // dummy variable for VirtualProtect

		// Finds the true address from addresses that require an offset from a module
		T* ResolveBase(T* offset)
		{
			using enum BaseAddress;
			switch (m_baseAddress)
			{
			case ABSOLUTE:
				return offset;
			case BZR:
				return m_bzrModuleBase + offset;
			case OGRE:
				return m_ogreMainModuleBase + offset;
			default:
				return nullptr;
			}
		}

	public:
		Scanner(T* address, Restore restoreData = Restore::ENABLED, 
			BaseAddress baseAddress = BaseAddress::ABSOLUTE)
			: m_address(ResolveBase(address)), m_restoreData(restoreData), m_baseAddress(baseAddress)
		{
			VirtualProtect(m_address, sizeof(T), PAGE_EXECUTE_READWRITE, &m_oldProtect);
			m_originalData = Read();
		}

		// Traverse multi-level pointer
		Scanner(T* address, const std::initializer_list<uint8_t>& offsetsList, Restore restoreData = Restore::ENABLED, 
			BaseAddress baseAddress = BaseAddress::ABSOLUTE)
			: m_address(ResolveBase(address)), m_restoreData(restoreData), m_baseAddress(baseAddress)
		{
			VirtualProtect(m_address, sizeof(T), PAGE_EXECUTE_READWRITE, &m_oldProtect);
			uintptr_t resolvedAddress = reinterpret_cast<uintptr_t>(m_address);
			std::vector offsets = offsetsList;
			for (size_t i = 0; i < offsets.size(); i++)
			{
				resolvedAddress = *reinterpret_cast<uintptr_t*>(resolvedAddress);
				resolvedAddress += offsets[i];
			}
			m_address = reinterpret_cast<T*>(resolvedAddress);
			m_originalData = Read();
		}

		Scanner(Scanner& o) = delete;
		Scanner(Scanner&& o) noexcept
			: BasicScanner(std::move(o))
		{
			this->m_baseAddress = o.m_baseAddress;
			this->m_address = o.m_address;
			this->m_restoreData = o.m_restoreData;
			this->m_originalData = o.m_originalData;
			this->m_oldProtect = o.m_oldProtect;
		}

		~Scanner()
		{
			if (m_restoreData == Restore::ENABLED)
			{
				Write(m_originalData);
			}
			VirtualProtect(m_address, sizeof(T), m_oldProtect, &m_dummyProtect);
		}

		T Read() const noexcept
		{
			if (m_address == nullptr)
			{
				return{};
			}

			return *m_address;
		}

		void Write(T value) noexcept
		{
			if (m_address == nullptr)
			{
				return;
			}

			*m_address = value;
		}

		// Get the underlying pointer of the data for direct access
		T* Get() const noexcept
		{
			if (m_address == nullptr)
			{
				return nullptr;
			}

			return m_address;
		}
	};
}