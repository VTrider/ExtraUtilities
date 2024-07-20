// dllmain.cpp : Defines the entry point for the DLL application.

#include "audiosystem.h"
#include "bzfunc.h"
#include "filesystem.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdint>
#include "dx9hook.h"
#include "Log.h"
#include "Memory.h"
#include <print>

#include "SoundBuffer.h"
#include "SoundDevice.h"
#include "SoundSource.h"

// Todo: memory flag to exit the thread, and free cursor frame game while gui is active

unsigned char* Hook(void* hookAddress, void* function, int length)
{
    if (length < 5) // need room for a jmp instruction
    {
        MessageBox(NULL, "You fool. You need at least 5 bytes to write a hook, go fix it before something breaks", "Uh Oh!", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
        return nullptr;
    }
    
    DWORD curProtection;
    VirtualProtect(hookAddress, length, PAGE_EXECUTE_READWRITE, &curProtection); // make sure you can write
    
    unsigned char* originalBytes = new unsigned char[length]; // for some reason this needs to be initialized to work in memcpy

    memcpy(originalBytes, hookAddress, length);

    memset(hookAddress, 0x90, length); // nops the hooked code so nothing bad happens

    DWORD relativeAddress = ((DWORD)function - (DWORD)hookAddress) - 5; // calculate the address of the function to jmp to

    *(BYTE*)hookAddress = 0xE9; // sets jmp instruction at hook address

    *(DWORD*)((DWORD)hookAddress + 1) = relativeAddress; // writes the next 4 bytes with the target address (opcode is 5 bytes total)

    DWORD temp;
    VirtualProtect(hookAddress, length, curProtection, &temp); // reset memory protection

    return originalBytes;
}

void Restore(void* address, unsigned char* originalBytes, int length)
{
    DWORD curProtection;
    VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &curProtection);

    memcpy(address, originalBytes, length);

    delete[] originalBytes;

    DWORD temp;
    VirtualProtect(address, length, curProtection, &temp);
}

/*---------------------------
* Weapon Mask Function Hook *
----------------------------*/

int weaponMask;

DWORD jmpBackWeaponMask;

void  __declspec(naked) WeaponMaskHook() // need to make sure the compiler doesn't mess with the asm
{
    __asm
    {
        // the value of the player's weapon mask is in the edx register at 
        // this location, this moves it into a variable before it executes 
        // the rest of the code normally
        mov [weaponMask],edx 
        mov [ecx+0x1C],edx
        mov eax,[ebp-0x00000110]
        jmp [jmpBackWeaponMask] // jumps back to the original code to resume normal execution
    }
}

/*---------
* Threads *
----------*/


void FileSystem()
{
    CreateEXUDirectory();
    std::cout << "EXU Directory Created" << '\n';

    SoundDevice* test = SoundDevice::Get();

    std::uint32_t sound = SoundBuffer::Get()->AddSoundEffect(".\\Extra Utilities\\rail.ogg");

    SoundSource speaker;

    speaker.Play(sound);

    SystemLog->Out("Got here");
}

bool enableConsole = false;

void InitializeConsole()
{
    if (enableConsole)
    {
        AllocConsole();
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        std::cout << "Console Initialized" << '\n';
    }
}

unsigned char* weaponMaskBytes;

void CodeInjection()
{
    // Weapon Mask Hook
    jmpBackWeaponMask = 0x0060A8C6 + 9;
    weaponMaskBytes = Hook((void*)0x0060A8C6, WeaponMaskHook, 9);
}

void Main()
{
    try
    {
        gui::Setup();
        hooks::Setup();
    }
    catch (const std::exception& error)
    {
        MessageBeep(MB_ICONERROR);
        MessageBox(
            0,
            error.what(),
            "exu error",
            MB_OK | MB_ICONEXCLAMATION
        );

        goto UNLOAD;
    }

    while (!GetAsyncKeyState(VK_END))
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

UNLOAD:
    hooks::Destroy();
    gui::Destroy();
}

Log* SystemLog;

// Make separate threads so windows doesn't freak out
DWORD WINAPI InitialThread(HMODULE hModule) 
{
    SystemLog = new Log();
    Memory::Init();

    InitializeConsole();
    FileSystem();
    CodeInjection();

    // std::thread MainThread(Main);
    // MainThread.detach();
    return 0;
}

extern bool ordnanceTweakApplied;
extern bool shotConvergenceApplied;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    { // make sure to break to avoid erroneous calls
    case DLL_PROCESS_ATTACH:
        //MessageBox(NULL, "ATTACH", "Uh Oh!", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitialThread, hModule, 0, nullptr));
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // MessageBox(NULL, "DETACH", "Uh Oh!", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
        // Resets hacked write protected values back to stock when the DLL is unloaded (when you leave the modded map)
        // Also resets values that don't reset because they aren't expecting to be changed
        Restore((void*)0x0060A8C6, weaponMaskBytes, 9);
        if (ordnanceTweakApplied)
        {
            Restore((void*)0x004803D4, ordnanceTweakBytes, 5);
            Restore((void*)0x00480357, ordnancePosBytes, 7);
        }
        if (shotConvergenceApplied)
        {
            Restore((void*)0x004eb590, shotConvergenceBytes, 6);
        }
        //MessageBox(NULL, "POST RESTORE", "Uh Oh!", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
        ResetValues();
        FreeConsole();
        SystemLog->Out("exu.dll detached from process");
        delete SystemLog;
        break;
    }
    return TRUE;
}