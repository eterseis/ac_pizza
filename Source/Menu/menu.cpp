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

void Menu::Render(int width, int height)
{
	ImGui::SetNextWindowSize(ImVec2(width * 0.6f, height * 0.6f));
	ImGui::Begin("null.win", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	static ImGuiTabItemFlags tabItemFlags{ ImGuiTabItemFlags_NoReorder | ImGuiTabItemFlags_NoTooltip | ImGuiTabItemFlags_NoCloseWithMiddleMouseButton };
	if (ImGui::BeginTabBar("myTabBar", ImGuiTabBarFlags_NoTooltip))
	{
		if (ImGui::BeginTabItem("Visuals", nullptr, tabItemFlags))
		{
			ImGui::BeginGroup();

			ImGui::InvisibleButton("padding0", { 1.0f, 8.0f });
			ImGui::InvisibleButton("padding1", { 8.0f, 1.0f });
			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::Text("ESP");
			ImGui::Checkbox("Enabled", &Settings::g_EnableVisuals);
			ImGui::Checkbox("Team Check", &Settings::g_Visuals_TeamCheck);
			ImGui::Checkbox("Outline", &Settings::g_Visuals_Outline);

			ImGui::EndGroup();

			ImGui::InvisibleButton("padding2", { 180.0f, 8.0f });
			ImGui::EndGroup();

			ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(30, 30, 30, 255), 3.0f);
			/**/

			ImGui::BeginGroup();

			ImGui::InvisibleButton("padding0", { 1.0f, 8.0f });
			ImGui::InvisibleButton("padding1", { 8.0f, 1.0f });
			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::Text("Snaplines");
			ImGui::Checkbox("Enabled", &Settings::g_EnableSnaplines);

			ImGui::EndGroup();

			ImGui::InvisibleButton("padding2", { 180.0f, 8.0f });
			ImGui::EndGroup();

			ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(30, 30, 30, 255), 3.0f);

			/**/
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