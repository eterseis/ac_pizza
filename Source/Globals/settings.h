#pragma once
#include "../Game/vector.h"

class Settings
{
public:
	bool SaveConfig(char* file_name);
	bool LoadConfig(char* file_name);

	bool g_ShowMenu{ false };
	bool g_EnableVisuals{ false };
	bool g_Visuals_TeamCheck{ false };
	bool g_Visuals_HealthCheck{ false };
	bool g_Visuals_Outline{ false };
	bool g_Visuals_Snaplines{ false };
	bool g_Visuals_BoundingBox{ false };
	bool g_Visuals_BoundingBoxFilled{ false };
	bool g_Visuals_HealthBar{ false };

	vec4 g_Visuals_Snaplines_Color{ 0.840f, 0.0f, 1.0f, 1.0f };
	vec4 g_BoundingBox_Color{ .x = 0.177f, .z = 1.0f, .w = 1.0f };
	vec4 g_FillBox_Color{ 0.910f, 0.039f, 1.0f, 0.199f };
	vec4 g_HealthBar_Color{ 0.0f, 1.0f, 0.818f, 1.0f };
};