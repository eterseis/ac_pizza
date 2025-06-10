#pragma once

#include "../Globals/settings.h"

#include "../Dependencies/ImGui/imgui.h"
#include "../Dependencies/ImGui/imgui_impl_glfw.h"
#include "../Dependencies/ImGui/imgui_impl_opengl3.h"

namespace Menu
{
	inline int selected_tab{ 0 };
	void LoadTheme(ImVec4* colors, ImGuiStyle& style);
	void Render(int width, int height);
}