#include "pch.hpp"
#include "includes.h"

static bool s_Active{ true };
static bool s_GlfwWindowFocused{ false };

void framebuffer_size_callback(GLFWwindow* w, int width, int height)
{
	glViewport(0, 0, width, height);
}

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

/* threads */
#pragma region
/* reading-writing */
void Puppeteer(uintptr_t* addresses, Game::Entity* ents, Game::Entity& myself, Visuals& v)
{
	while (s_Active)
	{
		Game::UpdateMyself(myself);
		Game::PopulateArray(addresses, ents);
		Memory::rpm<float[16]>(Globals::hProcess, Offsets::GetViewMatrix(), v.m_Matrix);
	}
}

void ProcessInput(GLFWwindow* window)
{
	using namespace std::chrono_literals;

	while (s_Active)
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
#pragma endregion

void LaunchThreads(uintptr_t* addresses, Game::Entity* ents, Game::Entity& myself, Visuals& v, GLFWwindow* window, bool& should_render)
{
	std::thread puppeteer(&Puppeteer, addresses, ents, std::ref(myself), std::ref(v));
	puppeteer.detach();

	std::thread process_input(&ProcessInput, window);
	process_input.detach();

	std::thread overlay_settings(&OverlaySettings, window, std::ref(should_render));
	overlay_settings.detach();
}

void Overlay(const uintptr_t* addresses, const Game::Entity* ents, const Game::Entity& myself, Visuals& v)
{
	if (!glfwInit())
	{
		MessageBoxA(nullptr, "failed to initialize glfw", "mozart.dead", MB_OK | MB_ICONERROR);
		return;
	}

	/* window hints */
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GL_FALSE);
		glfwWindowHint(GLFW_DECORATED, GL_FALSE);
		glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	}

	GLFWwindow* window{ glfwCreateWindow(800, 600, "mozart", nullptr, nullptr) };

	if (!window)
	{
		MessageBoxA(nullptr, "failed to create window", "mozart.dead", MB_OK | MB_ICONERROR);
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);

	/* hide taskbar icon */
	{
		glfwHideWindow(window);
		SetWindowLongA(glfwGetWin32Window(window), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
		glfwShowWindow(window);
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, GL_TRUE);
	}

	if (glewInit() != GLEW_OK)
	{
		MessageBoxA(nullptr, "failed to initialize glew", "mozart.dead", MB_OK | MB_ICONERROR);
		return;
	}

	Menu::Init(window);
	Menu::LoadTheme();

	bool should_render{ false };
	LaunchThreads(const_cast<uintptr_t*>(addresses), const_cast<Game::Entity*>(ents), const_cast<Game::Entity&>(myself), v, window, should_render);

	while (!glfwWindowShouldClose(window))
	{
		s_GlfwWindowFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwGetWindowSize(window, &v.m_Width, &v.m_Height);
		glClear(GL_COLOR_BUFFER_BIT);

		Menu::NewFrame();

		if (should_render)
		{
			if (Settings::g_ShowMenu)
			{
				Menu::Update(480, 360);
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

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_MULTISAMPLE);

		Menu::Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Menu::Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
	return;
}

int main()
{
	auto procId = Memory::GetProcessId("ac_client.exe");
	Globals::module_base = reinterpret_cast<uintptr_t>(Memory::GetModuleBaseAddress("ac_client.exe", procId));
	Globals::hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | SYNCHRONIZE, false, procId);

	AllocConsole();
	FILE* s;
	freopen_s(&s, "CONOUT$", "w", stdout);
	if (!Globals::hProcess)
	{
		MessageBoxA(0, "unable to find assault cube", "mozart.dead", MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}

	uintptr_t entityAddresses[31];
	Game::Entity* entities = new Game::Entity[31];
	Game::Entity myself;
	Visuals v{};

	Overlay(entityAddresses, entities, myself, v);

	CloseHandle(Globals::hProcess);
	delete[] entities;
	return EXIT_SUCCESS;
}