#include "menu.h"

void Menu::LoadTheme(ImVec4* colors, ImGuiStyle& style)
{
	ImVec4 gray{ 30 / 255.0f, 30 / 255.0f, 30 / 255.0f, 1.0f };
	ImVec4 dark{ 17 / 255.0f, 17 / 255.0f, 17 / 255.0f, 1.0f };
	ImVec4 pink{ 222 / 255.0f, 49 / 255.0f, 99 / 255.0f, 1.0f };
	ImVec4 light_gray{ gray.x, gray.y, gray.z, 0.5f };
	ImVec4 no_background{};

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = dark;
	colors[ImGuiCol_Border] = gray;
	colors[ImGuiCol_TitleBg] = dark;
	colors[ImGuiCol_TitleBgActive] = dark;

	colors[ImGuiCol_Tab] = no_background;
	colors[ImGuiCol_TabActive] = pink;
	colors[ImGuiCol_TabHovered] = pink;

	colors[ImGuiCol_FrameBg] = gray;
	colors[ImGuiCol_CheckMark] = pink;
	colors[ImGuiCol_FrameBgHovered] = light_gray;
	colors[ImGuiCol_FrameBgActive] = light_gray;

	style.WindowPadding = ImVec2(4.0f, 4.0f);
	style.WindowRounding = 3.0f;
	style.WindowBorderSize = 2.0f;
}

void Menu::BeginContainer(const char* label)
{
	ImGui::BeginGroup();

	ImGui::InvisibleButton(label + 1, { 1.0f, 8.0f });
	ImGui::InvisibleButton(label + 2, { 8.0f, 1.0f });
	ImGui::SameLine();

	ImGui::BeginGroup();
}

void Menu::EndContainer(const char* label)
{
	ImGui::EndGroup();

	ImGui::InvisibleButton(label + 3, { 180.0f, 8.0f });
	ImGui::EndGroup();

	ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(30, 30, 30, 255), 3.0f);
}

void Menu::Render(int width, int height)
{
	ImGui::SetNextWindowSize(ImVec2(width * 0.6f, height * 0.6f));
	ImGui::Begin("mozart.dead", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	static ImGuiTabItemFlags tabItemFlags{ ImGuiTabItemFlags_NoReorder | ImGuiTabItemFlags_NoTooltip | ImGuiTabItemFlags_NoCloseWithMiddleMouseButton };
	if (ImGui::BeginTabBar("myTabBar", ImGuiTabBarFlags_NoTooltip))
	{
		if (ImGui::BeginTabItem("Visuals", nullptr, tabItemFlags))
		{
			constexpr ImGuiColorEditFlags baseFlags{ ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar };

			Menu::BeginContainer("##VisualsSettings");
			ImGui::Text("ESP");
			ImGui::Checkbox("Enabled##0", &Settings::g_EnableVisuals);
			ImGui::Checkbox("Team Check", &Settings::g_Visuals_TeamCheck);
			ImGui::Checkbox("Health Check", &Settings::g_Visuals_HealthCheck);
			ImGui::Checkbox("Outline", &Settings::g_Visuals_Outline);
			Menu::EndContainer("##VisualsSettings");

			/*Snaplines*/
			Menu::BeginContainer("##Snaplines");
			ImGui::Text("Snaplines");
			ImGui::Checkbox("Enabled##1", &Settings::g_Visuals_Snaplines);
			ImGui::SameLine();
			ImGui::SetCursorPosX(width / 7.0f);
			ImGui::ColorEdit4("Snaplines Color", reinterpret_cast<float*>(&Settings::g_Visuals_Snaplines_Color), baseFlags);
			Menu::EndContainer("##Snaplines");

			/*BoundingBox*/

			Menu::BeginContainer("##BoundingBox");
			ImGui::Text("Bounding Box");
			ImGui::Checkbox("Enabled##2", &Settings::g_Visuals_BoundingBox);
			ImGui::SameLine();
			ImGui::SetCursorPosX(width / 7.0f);
			ImGui::ColorEdit4("Bounding Box Color", reinterpret_cast<float*>(&Settings::g_BoundingBox_Color), baseFlags);
			ImGui::Checkbox("Fill Box", &Settings::g_Visuals_BoundingBoxFilled);
			ImGui::SameLine();
			ImGui::SetCursorPosX(width / 7.0f);
			ImGui::ColorEdit4("Fill Box Color", reinterpret_cast<float*>(&Settings::g_FillBox_Color), baseFlags);
			Menu::EndContainer("##BoundingBox");

			/*Health Bar*/
			ImGui::SetCursorPos({ width / 4.0f, 46.0f });
			Menu::BeginContainer("##HealthBar");
			ImGui::Text("Health Bar");
			ImGui::Checkbox("Enabled##3", &Settings::g_Visuals_HealthBar);
			ImGui::SameLine();
			ImGui::SetCursorPosX(width / 2.6f);
			ImGui::ColorEdit4("Health Bar Color", reinterpret_cast<float*>(&Settings::g_HealthBar_Color), baseFlags);
			Menu::EndContainer("##HealthBar");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Aim", nullptr, tabItemFlags))
		{
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}