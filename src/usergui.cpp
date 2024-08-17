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

#include "bzfunc.h"

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

void SetupImGuiStyle()
{
	// Extra Utilities style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.1000000014901161f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 10.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(30.0f, 30.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 10.0f;
	style.PopupBorderSize = 0.0f;
	style.FramePadding = ImVec2(5.0f, 3.5f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(5.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 5.0f;
	style.ColumnsMinSpacing = 5.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 15.0f;
	style.GrabRounding = 5.0f;
	style.TabRounding = 5.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.3605149984359741f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 0.8669527769088745f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.4235294163227081f, 0.3803921639919281f, 0.572549045085907f, 0.54935622215271f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3803921639919281f, 0.4235294163227081f, 0.572549045085907f, 0.5490196347236633f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.4235294163227081f, 0.3803921639919281f, 0.572549045085907f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.4235294163227081f, 0.3803921639919281f, 0.572549045085907f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2588235437870026f, 0.2588235437870026f, 0.2588235437870026f, 0.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2352941185235977f, 0.2352941185235977f, 0.2352941185235977f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.8156862854957581f, 0.772549033164978f, 0.9647058844566345f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.4509803950786591f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 0.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.6196078658103943f, 0.5764706134796143f, 0.7686274647712708f, 0.5490196347236633f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.4235294163227081f, 0.3803921639919281f, 0.572549045085907f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.4235294163227081f, 0.3803921639919281f, 0.572549045085907f, 0.2918455004692078f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03433477878570557f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

bool showWelcome = true;

void WelcomeMenu()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Always, ImVec2(0.5, 0.5));
	ImGui::Begin("Extra Utilities by VTrider & more", &showWelcome, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Welcome! This map is a showcase of the features in Extra Utilities");

	ImGui::Separator();

	ImGui::Text(
		"This mod is the first public and open source\n"
		"script extender for Battlezone 1. It features over\n"
		"60 custom lua functions that expose hidden options,\n"
		"parameters, and much more to modders."
	);

	ImGui::SeparatorText("Highlighted Features");

	ImGui::BulletText("Custom gravity");
	ImGui::BulletText("Tweakable satellite parameters");
	ImGui::BulletText("Camera parameters: zoom/fov etc.");
	ImGui::BulletText("Set lives, difficulty, and other options");
	ImGui::BulletText("Read and modify TLI, auto level, etc.");
	ImGui::BulletText("Shot convergence on hovercraft, like bz2!");
	ImGui::BulletText("Ordnance velocity inheritance");
	ImGui::BulletText("Control unit selection, add, remove, etc.");
	ImGui::BulletText("Query a wealth of hidden values such as reticle position");
	ImGui::BulletText("Fully featured file I/O");
	ImGui::BulletText("Dynamic trn parameters: sunlight, fog, and more");
	ImGui::BulletText("Dynamic headlights: control brightness, color, etc.");
	ImGui::BulletText("Modern audio engine with support for sfx and mp3/ogg");

	ImGui::Separator();

	ImGui::Text(
		"The GUI you're looking at now is rendered entirely within the game\n"
		"and is powered by Dear ImGui.\n"
		"Note to modders: There's currently no lua bindings for this functionality\n"
		"so you'll have to make a fork of the project and compile your own dll\n"
		"in order to make a custom GUI");

	ImGui::SeparatorText("Usage");

	ImGui::Text(
		"Press right shift while in-game to access the menu/configurator\n"
		"Close this window when you're ready to proceed");

	ImGui::SeparatorText("Acknowledgements");

	ImGui::Text("Thanks to Scrap Pool for this awesome bzone font!");

	ImGui::Separator();

	ImGui::Text(
		"Special thanks to the players and modders of the\n"
		"Battlezone Community for their continued support,\n"
		"this wouldn't be possible without your help!");

	ImGui::Separator();

	ImGui::Text(
		"If you haven't already, be sure to join the Battlezone\n"
		"Community Discord at discord.gg/battlezonestrategy");

	ImGui::SeparatorText("Github Repo");

	ImGui::Text("https://github.com/VTrider/ExtraUtilities");

	ImGui::End();
}

bool setupStyle = false;
bool demoOpen = false;

void gui::UserGui() noexcept
{
	if (!setupStyle)
	{
		SetupImGuiStyle();
		setup = true;
	}

	if (bzoneFont)
	{
		ImGui::PushFont(bzoneFont);
	}

	if (showWelcome)
	{
		WelcomeMenu();
	}
	
	if (demoOpen)
	{
		ImGui::Begin("Extra Utilities Demo");
		ImGui::Text("Testing %f", Reticle::GetReticleAngle());
		ImGui::End();
		ImGui::ShowDemoWindow();
	}


	if (bzoneFont)
	{
		ImGui::PopFont();
	}
}

// depending on your implementation you might want to modify the wndproc callback
// in order to decide which inputs get passed through to the game while your ui
// is open, by default I have it block all game inputs as well as free the cursor
bool welcomeClosed = false;

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	// toggle menu
	if (welcomeClosed)
	{
		if (GetAsyncKeyState(VK_RSHIFT) & 1)
		{
			demoOpen = !demoOpen;
			if (demoOpen)
			{
				FreeCursor();
			}
			else
			{
				LockCursor();
			}
		}
	}

	if (showWelcome)
	{
		FreeCursor();
	}
	else if (!welcomeClosed)
	{
		LockCursor();
		welcomeClosed = true;
	}

	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam))
		return true;

	if (demoOpen or showWelcome) {

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