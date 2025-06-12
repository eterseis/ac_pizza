#include "pch.hpp"
#include "Memory/memory.hpp"
#include "Game/entity.h"
#include "Globals/globals.h"
#include "Features/Aimbot/aimbot.h"
#include "Features/Visuals/visuals.hpp"
#include "Math/math.hpp"
#include "Game/offsets.hpp"

#define GLEW_STATIC
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include "Dependencies/GLEW/GL/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/GLFW/glfw3native.h"
#include "Menu/menu.h"


static bool s_Active{ true };
static bool s_GlfwWindowFocused{ false };

void ForceWindowFocus(HWND window)
{
	INPUT input{};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VK_MENU;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;

	SendInput(1, &input, sizeof(INPUT));

	SetForegroundWindow(window);

	input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void ProcessInput(GLFWwindow* window)
{
	using namespace std::chrono_literals;

	while (true)
	{
		if (WaitForSingleObject(Globals::hProcess, 0) == WAIT_OBJECT_0)
		{
			s_Active = false;
			glfwSetWindowShouldClose(window, true);
		}

		if (GetAsyncKeyState(VK_END) & 1)
		{
			Settings::g_ShowMenu = !Settings::g_ShowMenu;
			(!Settings::g_ShowMenu) ? glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true) : glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);

			auto hwnd_ac{ FindWindowA(nullptr, "AssaultCube") };
			ForceWindowFocus(hwnd_ac);

			std::this_thread::sleep_for(20ms);
			if (Settings::g_ShowMenu)
			{
				ForceWindowFocus(glfwGetWin32Window(window));
			}
		}
		std::this_thread::sleep_for(1ms);
	}
}

/*actual reading-writing*/
void Puppeteer(std::array<uintptr_t, 31> addresses, Game::Entity* ents, Game::Entity& myself, Visuals& v)
{
	while (s_Active)
	{
		Game::UpdateMyself(myself);
		Game::PopulateArray(addresses, ents);
		//Aimbot::ClosestTarget(ents, myself);
		Memory::rpm<float[16]>(Globals::hProcess, Offsets::GetViewMatrix(), v.m_Matrix);
	}
}

void OverlaySettings(GLFWwindow* window, bool& should_render)
{
	using namespace std::chrono_literals;

	while (!glfwWindowShouldClose(window))
	{
		WINDOWINFO ac;
		ac.cbSize = sizeof(WINDOWINFO);

		auto hwnd_ac{ FindWindowA(0, "AssaultCube") };
		GetWindowInfo(hwnd_ac, &ac);

		glfwSetWindowPos(window, ac.rcClient.left, ac.rcClient.top);
		int width{ ac.rcClient.right - ac.rcClient.left };
		int height{ ac.rcClient.bottom - ac.rcClient.top };
		glfwSetWindowSize(window, width, height);

		(ac.dwWindowStatus || s_GlfwWindowFocused) ? should_render = true : should_render = false;
		std::this_thread::sleep_for(1ms);
	}
}

void Overlay(const char* title, const Game::Entity* ents, const Game::Entity& myself, Visuals& v)
{
	if (!glfwInit())
	{
		MessageBoxA(nullptr, "failed to initialize glfw", "mozart.dead", MB_OK | MB_ICONERROR);
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	/*overlay hints*/
	{
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GL_FALSE);
		glfwWindowHint(GLFW_DECORATED, GL_FALSE);
		glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	}

	GLFWwindow* window{ glfwCreateWindow(800, 600, title, nullptr, nullptr) };

	if (!window)
	{
		MessageBoxA(nullptr, "failed to create window", "mozart.dead", MB_OK | MB_ICONERROR);
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	/*****************************/
	/* hides window taskbar icon */
	glfwHideWindow(window);
	SetWindowLongA(glfwGetWin32Window(window), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	glfwShowWindow(window);
	glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GL_TRUE);
	/****************************/
	if (glewInit() != GLEW_OK)
	{
		MessageBoxA(nullptr, "failed to initialize glew", "mozart.dead", MB_OK | MB_ICONERROR);
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGuiStyle& style{ ImGui::GetStyle() };
	ImVec4* colors{ style.Colors };

	Menu::LoadTheme(colors, style);

	std::thread process_input(&ProcessInput, window);
	process_input.detach();

	bool should_render{ false };
	std::thread settings(&OverlaySettings, window, std::ref(should_render));
	settings.detach();

	while (!glfwWindowShouldClose(window))
	{
		s_GlfwWindowFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwGetWindowSize(window, &v.m_Width, &v.m_Height);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		if (should_render)
		{
			if (Settings::g_ShowMenu)
			{
				Menu::Render(v.m_Width, v.m_Height);
				{
					v.m_EnableVisuals = Settings::g_EnableVisuals;
					v.m_TeamCheck = Settings::g_Visuals_TeamCheck;
					v.m_HealthCheck = Settings::g_Visuals_HealthCheck;
					v.m_Outlined = Settings::g_Visuals_Outline;

					v.m_Snaplines = Settings::g_Visuals_Snaplines;
					v.m_BoundingBox = Settings::g_Visuals_BoundingBox;
					v.m_FillBox = Settings::g_Visuals_BoundingBoxFilled;
					v.m_HealthBar = Settings::g_Visuals_HealthBar;

					v.m_Snaplines_Color = Settings::g_Visuals_Snaplines_Color;
					v.m_BoundingBox_Color = Settings::g_BoundingBox_Color;
					v.m_FillBox_Color = Settings::g_FillBox_Color;
					v.m_HealthBar_Color = Settings::g_HealthBar_Color;
				}
			}

			v.Render(ents, myself);
		}

		ImGui::Render();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_MULTISAMPLE);
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return;
}

int main()
{
	auto procId = Memory::GetProcessId("ac_client.exe");
	Globals::module_base = reinterpret_cast<uintptr_t>(Memory::GetModuleBaseAddress("ac_client.exe", procId));
	Globals::hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | SYNCHRONIZE, false, procId);

	/*AllocConsole();
	FILE* s;
	freopen_s(&s, "CONOUT$", "w", stdout);*/
	if (!Globals::hProcess)
	{
		MessageBoxA(0, "unable to find assault cube", "mozart.dead", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	std::array<uintptr_t, 31> entityAddresses;
	Game::Entity* entities = new Game::Entity[31];
	Game::Entity myself;
	Visuals v{};

	std::thread pup(&Puppeteer, std::ref(entityAddresses), entities, std::ref(myself), std::ref(v));
	pup.detach();

	Overlay("mozart.dead", entities, myself, v);

	CloseHandle(Globals::hProcess);
	delete[] entities;
	return EXIT_SUCCESS;
}