
#include "gui.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

// write your custom ui here - widgets need to be within a window between imgui::begin and end,
// the demo window contains those already so if you are showing the demo they aren't necessary

void gui::UserGui() noexcept
{
	// ImGui::Begin("cool menu", &open);

	ImGui::ShowDemoWindow();

	// ImGui::End();
}