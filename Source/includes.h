#include "Memory/memory.hpp"
#include "Game/entity.h"
#include "Globals/globals.h"
#include "Features/Aimbot/aimbot.h"
#include "Features/Visuals/visuals.hpp"
#include "Math/math.hpp"
#include "Game/offsets.hpp"
#include "Menu/menu.h"
#include "timer.hpp"

#define GLEW_STATIC
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include "Dependencies/GLEW/GL/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/GLFW/glfw3native.h"