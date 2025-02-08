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

// dllmain.cpp : Defines the entry point for the DLL application.

#include <Windows.h>

#include <iostream>

static_assert(_WIN32); // BZR is 32 bit

HWND consoleWindow;

BOOL WINAPI DllMain(
    HINSTANCE hModule,
    DWORD fdwReason,
    LPVOID)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
#ifdef _DEBUG
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        consoleWindow = GetConsoleWindow();
        SetConsoleTitle("Extra Utilities Console");
#endif
        break;
    case DLL_PROCESS_DETACH:
#ifdef _DEBUG
        FreeConsole();
        PostMessage(consoleWindow, WM_CLOSE, 0, 0);
#endif
        break;
    }
    return TRUE;
}