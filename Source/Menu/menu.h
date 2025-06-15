#pragma once

#include "../Globals/settings.h"

#include "../Dependencies/ImGui/imgui.h"
#include "../Dependencies/ImGui/imgui_impl_glfw.h"
#include "../Dependencies/ImGui/imgui_impl_opengl3.h"

namespace Menu
{
	void BeginContainer(const char* id);
	void EndContainer(const char* id);
	void LoadTheme();

	void Init(GLFWwindow* window);
	void NewFrame();
	void Update(float width, float height, Settings& s);
	void Render();
	void Shutdown();
}