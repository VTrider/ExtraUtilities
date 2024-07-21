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

        std::uint32_t jmpBackAddress = static_cast<std::uint32_t>(hookAddress) + static_cast<std::uint32_t>(length);

        hookData.push_back({ hookAddress, originalBytes, length});

        memcpy(originalBytes, reinterpret_cast<void*>(hookAddress), length);

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
        for (auto& data : hookData)
        {
            Restore(data.hookAddress, data.originalBytes, data.length);
        }
    }

};