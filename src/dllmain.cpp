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

// dllmain.cpp : Defines the entry point for the DLL application.

#pragma warning(disable : 6387) // suppress warning on create thread, yes I know it's dangerous lol

#define WIN32_LEAN_AND_MEAN

#include "asm.h"
#include "Audio.h"
#include "bzfunc.h"
#include "dx9hook.h"
#include "filesystem.h"
#include "Hook.h"
#include "Log.h"
#include "Memory.h"
#include "Offsets.h"
#include "SoundDevice.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>
#include <Windows.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

/*---------
* Threads *
----------*/

static void FileSystem()
{
    CreateEXUDirectory();
}

static void CodeInjection()
{
    Hook::CreateHook(Hooks::weaponMask, &WeaponMaskHook, 9);
    Hook::CreateHook(Hooks::ordnanceVelocity, &OrdnanceVelocityHook, 5);
    Hook::CreateHook(Hooks::ordnancePosition, &OrdnancePositionHook, 7);
    Hook::CreateHook(Hooks::shotConvergence, &ShotConvergenceHook, 9);
    Hook::CreateHook(Hooks::getLightPtr, &LightPtrHook, 6);
    Hook::CreateHook(Hooks::selectNone, &SelectNoneHook, 6);
    Hook::CreateHook(Hooks::bulletHitCB, &BulletHitCallback, 5);
    Hook::CreateHook(fogHook, &FogHook, 9);
}

//static void GUI()
//{
//    SystemLog->Out("Started GUI thread", 3);
//    try
//    {
//        gui::Setup();
//        hooks::Setup();
//    }
//    catch (const std::exception& error)
//    {
//        MessageBeep(MB_ICONERROR);
//        MessageBox(
//            0,
//            error.what(),
//            "exu error",
//            MB_OK | MB_ICONEXCLAMATION
//        );
//
//        goto UNLOAD;
//    }
//
//    while (true)
//    {
//        if (Memory::CheckExitCondition(5, "Exit condition detected, exiting GUI thread"))
//        {
//            break;
//        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(5));
//    }
//
//UNLOAD:
//    hooks::Destroy();
//    gui::Destroy();
//}

std::unique_ptr<Log> SystemLog;

static void AudioThread()
{
    SystemLog->Out("Started audio thread", 3);
    SoundDevice device = SoundDevice();

    while (true)
    {
        if (Memory::CheckExitCondition(5, "Exit condition detected, exiting audio thread"))
        {
            break;
        }

        Audio::ProcessSoundRequests();
        Audio::CleanSources();



        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

// it's okay to detach these threads cause they will stop automatically
static DWORD WINAPI InitialThread(HMODULE) 
{
    SystemLog = std::make_unique<Log>(Log());

    int level = SystemLog->GetLevel();
    std::string levelString;
    switch (level)
    {
    case 0:
        levelString = "OFF";
        break;
    case 1:
        levelString = "ERROR";
        break;
    case 2:
        levelString = "WARNING";
        break;
    case 3:
        levelString = "INFO";
        break;
    }
    SystemLog->Out(std::format("Extra Utilities started successfully! Version: {}", Exu::version));
    SystemLog->Out(std::format("Logging level is: {}", levelString));

    Memory::Init();

    FileSystem();
    CodeInjection();

    std::thread audioThread(AudioThread);
    audioThread.detach();

    // std::thread GUIThread(GUI);
    // GUIThread.detach();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitialThread, hModule, 0, nullptr));
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Resets hacked write protected values back to stock when the DLL is unloaded (when you leave the modded map)
        // Also resets values that don't reset because they aren't expecting to be changed
        Hook::RestoreAll();
        Memory::RestoreAll();
        SystemLog->Out("exu.dll detached from process", 3);
        break;
    }
    return TRUE;
}