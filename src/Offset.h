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
* Memory Reader class with automatic
* protection and restoration of the original
* data
*/

#pragma once

#include <Windows.h>

namespace ExtraUtilities
{
	template <class T>
	class Offset
	{
	private:
		T* m_address;
		T m_originalData;
		DWORD m_oldProtect{};
		static inline DWORD dummyProtect;

	public:
		Offset(T* address) : m_address(address)
		{
			VirtualProtect(m_address, sizeof(T), PAGE_EXECUTE_READWRITE, &m_oldProtect);
			m_originalData = Read();
		}

		Offset(Offset& o) = delete;
		Offset(Offset&& o) = delete;

		~Offset()
		{
			Write(m_originalData);
			VirtualProtect(m_address, sizeof(T), m_oldProtect, &dummyProtect);
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