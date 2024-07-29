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

#include "Offsets.h"

#include "gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParam, LPARAM longParam);

DWORD temp;
// this is insanely hacky, couldn't hook here so I'm just writing the opcode
// to be je (lock) or jne (unlock) lol
void FreeCursor()
{
	VirtualProtect(reinterpret_cast<void*>(Hooks::freeCursor), 1, PAGE_EXECUTE_READWRITE, &temp);
	*reinterpret_cast<char*>(Hooks::freeCursor) = 0x75;
	while (ShowCursor(TRUE) <= 0); // need to loop because it increments a counter not setting a direct value
}

void LockCursor()
{
	VirtualProtect(reinterpret_cast<void*>(Hooks::freeCursor), 1, PAGE_EXECUTE_READWRITE, &temp);
	*reinterpret_cast<char*>(Hooks::freeCursor) = 0x74;
	while (ShowCursor(FALSE) >= 0);
}

// write your custom ui here - widgets need to be within a window between imgui::begin and end,
// the demo window contains those already so if you are showing the demo they aren't necessary

void gui::UserGui() noexcept
{
	// ImGui::Begin("cool menu", &open);

	ImGui::ShowDemoWindow();

	// ImGui::End();
}

// depending on your implementation you might want to modify the wndproc callback
// in order to decide which inputs get passed through to the game while your ui
// is open, by default I have it block all game inputs as well as free the cursor

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	// toggle menu
	if (GetAsyncKeyState(VK_RSHIFT) & 1)
	{
		gui::open = !gui::open;
		if (gui::open)
		{
			FreeCursor();
		}
		else
		{
			LockCursor();
		}
	}

	if (gui::open) {
		if (ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam))
			return true;

		// Block all keyboard and mouse input messages from reaching the game when the ui is open,
		// specifically if an input wasn't handled by imgui, yet the ui is still open, then it
		// must be a "game input" so it should be blocked
		if (message == WM_MOUSEMOVE
			or message == WM_MOUSEWHEEL
			or message == WM_INPUT
			or message == WM_CHAR
			or message == WM_SETCURSOR
			or message == WM_KEYDOWN
			or message == WM_KEYUP
			or message == WM_SYSKEYDOWN
			or message == WM_SYSKEYUP)
		{
			return 0;
		}
	}

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}