#pragma once
#include "../Game/vector.h"

namespace Settings
{
	inline bool g_ShowMenu{ false };

	inline bool g_EnableVisuals{ true };
	inline bool g_Visuals_TeamCheck{ true };
	inline bool g_Visuals_HealthCheck{ true };
	inline bool g_Visuals_Outline{ true };

	inline bool g_Visuals_Snaplines{ true };
	inline vec4 g_Visuals_Snaplines_Color{ .x = 1.0f, .w = 1.0f };

	inline bool g_Visuals_BoundingBox{ true };
	inline vec4 g_BoundingBox_Color{ 222 / 255.0f, 49 / 255.0f, 99 / 255.0f, 1.0f };
	inline bool g_Visuals_BoundingBoxFilled{ true };
	inline vec4 g_FillBox_Color{ 222 / 255.0f, 49 / 255.0f, 99 / 255.0f, 0.4f };

	inline bool g_Visuals_HealthBar{ true };
	inline vec4 g_HealthBar_Color{ .y = 1.0f, .w = 1.0f };
}