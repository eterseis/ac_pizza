#include "pch.hpp"
#include "Memory/memory.hpp"
#include "Game/entity.h"
#include "Globals/globals.h"
#include "Features/Aimbot/aimbot.h"
#include "Features/Visuals/visuals.hpp"
#include "Math/math.hpp"
#include "Game/offsets.hpp"

#define GLEW_STATIC

#include "Dependencies/GLEW/GL/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Menu/menu.h"


static bool s_active{ true };

class Timer
{
public:
	Timer()
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		Stop();
	}
	void Stop()
	{
		auto endTimePoint{ std::chrono::high_resolution_clock::now() };

		auto start{ std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count() };
		auto end{ std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count() };

		auto duration{ end - start };
		double ms{ duration * 0.001 };

		std::cout << duration << "us (" << ms << "ms)\n";
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
};

void Aim(const Game::Entity* ents, const Game::Entity& myself)
{
	const Game::Entity* pTarget = Game::ClosestEntity(ents, myself);
	Aimbot::ClosestTarget(*pTarget, myself);
}

void ProcessInput(GLFWwindow* window)
{
	while (true)
	{
		if (WaitForSingleObject(Globals::hProcess, 0) == WAIT_OBJECT_0)
		{
			s_active = false;
			glfwSetWindowShouldClose(window, true);
		}

		if (GetAsyncKeyState(VK_END) & 1)
		{
			Settings::g_ShowMenu = !Settings::g_ShowMenu;
		}
	}
}

/*actual reading-writing*/
void Puppeteer(std::array<uintptr_t, 31> addresses, Game::Entity* ents, Game::Entity& myself, Visuals& v)
{
	while (s_active)
	{
		Game::UpdateMyself(myself);
		Game::PopulateArray(addresses, ents);
		//Aim(ents, myself);
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


		(!ac.dwWindowStatus) ? should_render = false : should_render = true;

		std::this_thread::sleep_for(1ms);
	}
}

void Overlay(const char* title, const Game::Entity* ents, const Game::Entity& myself, Visuals& v)
{
	if (!glfwInit())
	{
		MessageBoxA(nullptr, "failed to initialize glfw", "Null", MB_OK | MB_ICONERROR);
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GL_TRUE);

	GLFWwindow* window{ glfwCreateWindow(800, 600, title, nullptr, nullptr) };

	if (!window)
	{
		MessageBoxA(nullptr, "failed to create window", "Null", MB_OK | MB_ICONERROR);
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		MessageBoxA(nullptr, "failed to initialize glew", "Null", MB_OK | MB_ICONERROR);
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
		glfwGetWindowSize(window, &v.m_Width, &v.m_Height);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (should_render)
		{
			if (Settings::g_ShowMenu)
			{
				//ImGui::ShowDemoWindow();
				Menu::Render(v.m_Width, v.m_Height);
			}
			v.Render(ents);
		}

		ImGui::Render();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
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
		MessageBoxA(0, "unable to find assault cube", "Null", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	std::array<uintptr_t, 31> entityAddresses;
	Game::Entity* entities = new Game::Entity[31];
	Game::Entity myself;
	Visuals v{ .m_Outlined = true };

	std::thread pup(&Puppeteer, std::ref(entityAddresses), entities, std::ref(myself), std::ref(v));
	pup.detach();

	Overlay("Null", entities, myself, v);

	CloseHandle(Globals::hProcess);
	delete[] entities;
	return EXIT_SUCCESS;
}