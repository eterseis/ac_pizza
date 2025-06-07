#include "pch.hpp"
#include "Memory/memory.hpp"
#include "Game/entity.h"
#include "Globals/globals.h"
#include "Features/Aimbot/aimbot.h"
#include "Features/Visuals/visuals.hpp"
#include "Math/math.hpp"
#include "Game/offsets.hpp"

#define GLEW_STATIC

#include "Dependencies/GLEW/glew.h"
#include "Dependencies/GLFW/glfw3.h"


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
	if (WaitForSingleObject(Globals::hProcess, 0) == WAIT_OBJECT_0)
	{
		s_active = false;
		glfwSetWindowShouldClose(window, true);
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

void OverlaySettings(GLFWwindow* window)
{
	using namespace std::chrono_literals;

	while (!glfwWindowShouldClose(window))
	{
		//glfwHideWindow(window);

		WINDOWINFO ac;
		ac.cbSize = sizeof(WINDOWINFO);

		auto hwnd_ac{ FindWindowA(0, "AssaultCube") };
		GetWindowInfo(hwnd_ac, &ac);

		glfwSetWindowPos(window, ac.rcClient.left, ac.rcClient.top);
		int width{ ac.rcClient.right - ac.rcClient.left };
		int height{ ac.rcClient.bottom - ac.rcClient.top };
		glfwSetWindowSize(window, width, height);

		std::this_thread::sleep_for(1ms);
	}
}

void Overlay(const char* title, const Game::Entity* ents, const Game::Entity& myself, Visuals& v)
{
	if (!glfwInit())
	{
		std::cout << "failed to initialize glfw\n";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GL_TRUE);

	GLFWwindow* window{ glfwCreateWindow(800, 600, title, nullptr, nullptr) };

	if (!window)
	{
		std::cout << "failed to create window\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "failed to initialize glew\n";
		return;
	}

	std::thread settings(&OverlaySettings, window);
	settings.detach();

	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwGetWindowSize(window, &v.m_Width, &v.m_Height);
		{
			Timer t;
			v.TestViewMatrix(ents);

		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

int main()
{
	auto procId = Memory::GetProcessId("ac_client.exe");
	Globals::module_base = reinterpret_cast<uintptr_t>(Memory::GetModuleBaseAddress("ac_client.exe", procId));
	Globals::hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | SYNCHRONIZE, false, procId);

	if (!Globals::hProcess)
	{
		std::cout << "failed to find assault cube process\n";
		return -1;
	}

	std::array<uintptr_t, 31> entityAddresses;
	Game::Entity* entities = new Game::Entity[31];
	Game::Entity myself;
	Visuals v{ .m_Outlined = true };

	std::thread pup(&Puppeteer, std::ref(entityAddresses), entities, std::ref(myself), std::ref(v));
	pup.detach();

	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	Overlay("Cosmo", entities, myself, v);

	CloseHandle(Globals::hProcess);
	delete[] entities;
	return 0;
}