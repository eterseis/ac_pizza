#pragma once

#include "../Globals/settings.h"

#include "../Dependencies/ImGui/imgui.h"
#include "../Dependencies/ImGui/imgui_impl_glfw.h"
#include "../Dependencies/ImGui/imgui_impl_opengl3.h"

namespace Menu
{
	void LoadTheme(ImVec4* colors, ImGuiStyle& style);
	void BeginContainer(const char* label);
	void EndContainer(const char* label);
	void Render(int width, int height);
}