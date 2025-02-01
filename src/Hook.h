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

#pragma once

#include <Windows.h>

#include <cstdint>
#include <format>
#include <string>
#include <vector>

namespace ExtraUtilities
{
	class Hook
	{
	private:
		bool m_active = false;

		uintptr_t m_address;
		uint32_t m_length;
		void* m_function;

		DWORD m_oldProtect{};
		DWORD dummyProtect{};
		std::vector<uint8_t> m_originalBytes;

		static constexpr uint8_t NOP = 0x90;
		static constexpr uint8_t JMP = 0xE9;

		bool ValidateHook() const
		{
			if (m_length < 5)
			{
				std::string error = std::format("Extra Utilities Error: Not enough space for hook at address {}", m_address);
				MessageBox(0, error.c_str(), "Uh Oh!", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
				return false;
			}
			return true;
		}

		void DoHook()
		{
			uint8_t* p_address = reinterpret_cast<uint8_t*>(m_address);

			VirtualProtect(p_address, m_length, PAGE_EXECUTE_READWRITE, &m_oldProtect);

			m_originalBytes.insert(m_originalBytes.end(), p_address, p_address + m_length);

			std::memset(p_address, NOP, m_length);
			std::memset(p_address, JMP, 1);

			ptrdiff_t relativeAddress = reinterpret_cast<uintptr_t>(m_function) - m_address - 5;
			std::memcpy(p_address + 1, &relativeAddress, sizeof(relativeAddress));

			VirtualProtect(p_address, m_length, m_oldProtect, &dummyProtect);

			m_active = true;
		}

		void RestoreHook()
		{
			uint8_t* p_address = reinterpret_cast<uint8_t*>(m_address);

			VirtualProtect(p_address, m_length, PAGE_EXECUTE_READWRITE, &m_oldProtect);

			std::memcpy(p_address, m_originalBytes.data(), m_length);

			VirtualProtect(p_address, m_length, m_oldProtect, &dummyProtect);

			m_active = false;
		}

	public:
		Hook(uintptr_t address, uint32_t length, void* function)
			: m_address(address), m_length(length), m_function(function)
		{
			if (!ValidateHook())
			{
				return;
			}

			DoHook();
		}

		Hook(Hook& h) = delete; // hook should not be instantiated twice

		Hook(Hook&& h) noexcept
		{
			this->m_address = h.m_address;
			this->m_length = h.m_length;
			this->m_function = h.m_function;
			this->m_oldProtect = h.m_oldProtect;
			this->dummyProtect = h.dummyProtect;
			this->m_originalBytes = std::move(h.m_originalBytes);
		}

		~Hook()
		{
			RestoreHook();
		}

		void Reload()
		{
			if (!m_active)
			{
				DoHook();
			}
		}

		void Unload()
		{
			if (m_active)
			{
				RestoreHook();
			}
		}
	};
}