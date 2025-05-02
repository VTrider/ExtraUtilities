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

#include <Windows.h>

#include <cstdint>
#include <vector>

namespace ExtraUtilities
{
	class BasicPatch
	{
	public:
		enum class Status : uint8_t
		{
			ACTIVE, // patched code is running
			INACTIVE // game code is running
		};

		// x86 shellcode
		static constexpr uint8_t NOP = 0x90; // no operation
		static constexpr uint8_t RET = 0xC3; // return

	protected:
		Status m_status;

		uintptr_t m_address;
		size_t m_length;

		DWORD m_oldProtect{};
		static inline DWORD dummyProtect{};
		std::vector<uint8_t> m_originalBytes;

		virtual void DoPatch() = 0;

		void RestorePatch()
		{
			uint8_t* p_address = reinterpret_cast<uint8_t*>(m_address);

			VirtualProtect(p_address, m_length, PAGE_EXECUTE_READWRITE, &m_oldProtect);

			std::memcpy(p_address, m_originalBytes.data(), m_length);

			VirtualProtect(p_address, m_length, m_oldProtect, &dummyProtect);

			m_status = Status::INACTIVE;
		}

	public:
		BasicPatch(uintptr_t address, size_t length, Status status)
			: m_address(address), m_length(length), m_status(status)
		{
			uint8_t* p_address = reinterpret_cast<uint8_t*>(m_address);

			VirtualProtect(p_address, m_length, PAGE_EXECUTE_READWRITE, &m_oldProtect);

			m_originalBytes.insert(m_originalBytes.end(), p_address, p_address + m_length);

			VirtualProtect(p_address, m_length, m_oldProtect, &dummyProtect);
		}

		BasicPatch(BasicPatch& p) = delete; // Patch should not be initialized twice

		BasicPatch(BasicPatch&& p) noexcept
		{
			this->m_status = p.m_status;
			this->m_address = p.m_address;
			this->m_length = p.m_length;
			this->m_oldProtect = p.m_oldProtect;
			this->dummyProtect = p.dummyProtect;
			this->m_originalBytes = std::move(p.m_originalBytes);
		}

		virtual ~BasicPatch()
		{
			RestorePatch();
		}

		bool IsActive()
		{
			return m_status == Status::ACTIVE ? true : false;
		}

		void Reload()
		{
			if (m_status == Status::INACTIVE)
			{
				DoPatch();
			}
		}

		void Unload()
		{
			if (m_status == Status::ACTIVE)
			{
				RestorePatch();
			}
		}

		void SetStatus(Status s)
		{
			switch (s)
			{
			case Status::ACTIVE:
				Reload();
				break;

			case Status::INACTIVE:
				Unload();
				break;
			}
		}

		void SetStatus(bool status)
		{
			SetStatus(static_cast<Status>(!status));
		}
	};
}