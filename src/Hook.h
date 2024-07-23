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

#include <iostream>
#include <vector>

struct HookData
{
    std::uintptr_t hookAddress;
    unsigned char* originalBytes;
    int length;
};

class Hook
{
private:

    static inline std::vector<HookData> hookData{};

    static void Restore(std::uintptr_t address, unsigned char* originalBytes, int length)
    {
        DWORD curProtection;
        VirtualProtect(reinterpret_cast<void*>(address), length, PAGE_EXECUTE_READWRITE, &curProtection);

        memcpy(reinterpret_cast<void*>(address), originalBytes, length);

        delete[] originalBytes;

        DWORD temp;
        VirtualProtect(reinterpret_cast<void*>(address), length, curProtection, &temp);
    }

public:

    // WARNING: be extremely careful and triple check that you counted the right number of fking bytes,
    // if the hook is crashing despite proper usage, that probably means you interrupted some critical
    // function so you need to hook a different location
    static void CreateHook(std::uintptr_t hookAddress, void* function, int length)
    {
        if (length < 5) // need room for a jmp instruction
        {
            MessageBox(NULL, "You fool. You need at least 5 bytes to write a hook, go fix it before something breaks", "Uh Oh!", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
            return;
        }

        DWORD curProtection;
        VirtualProtect(reinterpret_cast<void*>(hookAddress), length, PAGE_EXECUTE_READWRITE, &curProtection);

        unsigned char* originalBytes = new unsigned char[length];
        memcpy(originalBytes, reinterpret_cast<void*>(hookAddress), length);
        hookData.push_back({ hookAddress, originalBytes, length });

        memset(reinterpret_cast<void*>(hookAddress), 0x90, length); // nops the hooked code so nothing bad happens

        DWORD relativeAddress = ((DWORD)function - (DWORD)hookAddress) - 5; // calculate the address of the function to jmp to
        *(BYTE*)hookAddress = 0xE9; // sets jmp instruction at hook address
        *(DWORD*)((DWORD)hookAddress + 1) = relativeAddress; // writes the next 4 bytes with the target address (opcode is 5 bytes total)

        DWORD temp;
        VirtualProtect(reinterpret_cast<void*>(hookAddress), length, curProtection, &temp);
    }

    // you must ensure this gets called when the DLL unloads, otherwise you will insta crash
    // if you load into a stock map
    static void RestoreAll()
    {
        for (const auto& data : hookData)
        {
            Restore(data.hookAddress, data.originalBytes, data.length);
        }
    }

};