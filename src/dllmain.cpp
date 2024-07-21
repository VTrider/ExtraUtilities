// dllmain.cpp : Defines the entry point for the DLL application.

#include "Audio.h"
#include "bzfunc.h"
#include "filesystem.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "dx9hook.h"
#include "Log.h"
#include "Memory.h"
#include <print>
#include "Offsets.h"
#include "Hook.h"
#include "asm.h"


#include "SoundBuffer.h"
#include "SoundDevice.h"
#include "SoundSource.h"

// Todo: memory flag to exit the thread, and free cursor frame game while gui is active
// fix open AL holding on to a handle or something

/*---------
* Threads *
----------*/

void FileSystem()
{
    CreateEXUDirectory();
    std::cout << "EXU Directory Created" << '\n';
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
    Hook::CreateHook(Hooks::weaponMask, WeaponMaskHook, 9);
    Hook::CreateHook(Hooks::ordnanceVelocity, OrdnanceVelocityHook, 5);
    Hook::CreateHook(Hooks::ordnancePosition, OrdnancePositionHook, 7);
    Hook::CreateHook(Hooks::shotConvergence, ShotConvergenceHook, 6);
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

void AudioSystem()
{
    Audio audioSystem;

    while (true)
    {
        if (Memory::CheckExitCondition(5))
        {
            break;
        }
    

    
        
        
    }
}

// Make separate threads so windows doesn't freak out
DWORD WINAPI InitialThread(HMODULE hModule) 
{
    SystemLog = new Log();
    Memory::Init();
    InitializeConsole();
    FileSystem();
    CodeInjection();
    
    // std::thread audio(AudioSystem);
    // audio.detach();

    // std::thread MainThread(Main);
    // MainThread.detach();
    return 0;
}

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
        Hook::RestoreAll();
        ResetValues();
        FreeConsole();
        SystemLog->Out("exu.dll detached from process", 3);
        delete SystemLog;
        MessageBox(NULL, "PROCESS DETACH!", "Uh Oh!", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
        break;
    }
    return TRUE;
}