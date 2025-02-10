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

#include "BasicPatch.h"

#include <Windows.h>

#include <format>

namespace ExtraUtilities
{
	class Hook : public BasicPatch
	{
	private:
		const void* m_function;

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

		void DoPatch() override
		{
			uint8_t* p_address = reinterpret_cast<uint8_t*>(m_address);

			VirtualProtect(p_address, m_length, PAGE_EXECUTE_READWRITE, &m_oldProtect);

			std::memset(p_address, NOP, m_length);
			std::memset(p_address, JMP, 1);

			ptrdiff_t relativeAddress = reinterpret_cast<uintptr_t>(m_function) - m_address - 5;
			// +1 byte because the first is the jmp instruction, next 4 the address
			std::memcpy(p_address + 1, &relativeAddress, sizeof(relativeAddress)); 

			VirtualProtect(p_address, m_length, m_oldProtect, &dummyProtect);

			m_active = true;
		}

	public:
		Hook(uintptr_t address, const void* function, size_t length, bool startActive = true)
			: BasicPatch(address, length, startActive), m_function(function)
		{
			if (!ValidateHook())
			{
				return;
			}

			if (m_active)
			{
				DoPatch();
			}
		}

		Hook(Hook& h) = delete;

		Hook(Hook&& h) noexcept
			: BasicPatch(std::move(h))
		{
			this->m_function = h.m_function;
		}

		~Hook() = default;
	};
}
