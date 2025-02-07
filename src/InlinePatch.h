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

#include <vector>

namespace ExtraUtilities
{
	template <class T>
	class InlinePatch : public BasicPatch
	{
	private:
		std::vector<uint8_t> m_payload;

		void DoPatch() override
		{
			uint8_t* p_address = reinterpret_cast<uint8_t*>(m_address);

			VirtualProtect(p_address, m_length, PAGE_EXECUTE_READWRITE, &m_oldProtect);

			m_originalBytes.insert(m_originalBytes.end(), p_address, p_address + m_length);

			std::memcpy(p_address, m_payload.data(), m_length);

			VirtualProtect(p_address, m_length, m_oldProtect, &dummyProtect);

			m_active = true;
		}

	public:
		InlinePatch(uintptr_t address, T* payload, size_t length, bool startActive = true)
			: BasicPatch(address, length, startActive), m_payload((uint8_t*)payload, (uint8_t*)payload + length)
		{
			DoPatch();
		}

		InlinePatch(InlinePatch& p) = delete;

		InlinePatch(InlinePatch&& p) noexcept
			: BasicPatch(std::move(p))
		{
			this->m_payload = std::move(p.m_payload);
		}

		~InlinePatch() = default;
	};
}