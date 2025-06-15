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

void ProcessInput(GLFWwindow* window, Settings& s)
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
			s.g_ShowMenu = !s.g_ShowMenu;
			(!s.g_ShowMenu) ? glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true) : glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);

			auto hwnd_ac{ FindWindowA(nullptr, "AssaultCube") };
			ForceWindowFocus(hwnd_ac);

			std::this_thread::sleep_for(20ms);
			if (s.g_ShowMenu)
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

void LaunchThreads(uintptr_t* addresses, Game::Entity* ents, Game::Entity& myself, Visuals& v, GLFWwindow* window, bool& should_render, Settings& s)
{
	std::thread puppeteer(&Puppeteer, addresses, ents, std::ref(myself), std::ref(v));
	puppeteer.detach();

	std::thread process_input(&ProcessInput, window, std::ref(s));
	process_input.detach();

	std::thread overlay_settings(&OverlaySettings, window, std::ref(should_render));
	overlay_settings.detach();
}

void FreeShit()
{
	struct Character
	{
		unsigned int TextureId;
		vec2 Size;
		vec2 Bearing;
		unsigned int Advance;
	};

	static std::map<char, Character> characters;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: could not init library\n";
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, "C:/Windows/Fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: failed to load font\n";
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYPE: failed to load glyph\n";
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c{}; c < 128; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: failed to load glyph\n";
			continue;
		}
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character{
			texture,
			{(float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows},
			{(float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top},
			(unsigned int)face->glyph->advance.x
		};
		characters.insert(std::pair<char, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Overlay(const uintptr_t* addresses, const Game::Entity* ents, const Game::Entity& myself, Visuals& v, Settings& settings)
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
	LaunchThreads(
		const_cast<uintptr_t*>(addresses),
		const_cast<Game::Entity*>(ents),
		const_cast<Game::Entity&>(myself),
		v,
		window,
		should_render,
		settings
	);

	//FreeShit();
	while (!glfwWindowShouldClose(window))
	{
		s_GlfwWindowFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwGetWindowSize(window, &v.m_Width, &v.m_Height);
		glClear(GL_COLOR_BUFFER_BIT);

		Menu::NewFrame();

		if (should_render)
		{
			if (settings.g_ShowMenu)
			{
				Menu::Update(360, 480, settings);
				{
					v.m_EnableVisuals = settings.g_EnableVisuals;
					v.m_TeamCheck = settings.g_Visuals_TeamCheck;
					v.m_HealthCheck = settings.g_Visuals_HealthCheck;
					v.m_Outlined = settings.g_Visuals_Outline;

					v.m_Snaplines = settings.g_Visuals_Snaplines;
					v.m_BoundingBox = settings.g_Visuals_BoundingBox;
					v.m_FillBox = settings.g_Visuals_BoundingBoxFilled;
					v.m_HealthBar = settings.g_Visuals_HealthBar;

					v.m_Snaplines_Color = settings.g_Visuals_Snaplines_Color;
					v.m_BoundingBox_Color = settings.g_BoundingBox_Color;
					v.m_FillBox_Color = settings.g_FillBox_Color;
					v.m_HealthBar_Color = settings.g_HealthBar_Color;
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
	Settings settings{};

	Overlay(entityAddresses, entities, myself, v, settings);

	CloseHandle(Globals::hProcess);
	delete[] entities;
	return EXIT_SUCCESS;
}