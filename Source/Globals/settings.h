#pragma once
#include "../Game/vector.h"

class Settings
{
public:
	bool SaveConfig(char* file_name);
	bool LoadConfig(char* file_name);

	bool m_ShowMenu{ false };
	bool m_EnableVisuals{ true };
	bool m_Visuals_TeamCheck{ false };
	bool m_Visuals_HealthCheck{ false };
	bool m_Visuals_Outline{ false };
	bool m_Visuals_Snaplines{ false };
	bool m_Visuals_BoundingBox{ false };
	bool m_Visuals_BoundingBoxFilled{ false };
	bool m_Visuals_HealthBar{ true };
	bool m_Visuals_Name{ false };

	bool m_EnableAim{ false };
	bool m_Aim_EnemyOnly{ false };
	bool m_ClosestEntity{ false };

	bool m_Misc_ClientMods{ false };
	bool m_Misc_UnlimitedHealth{ false };
	bool m_Misc_MaxArmor{ false };

	bool m_Misc_WeaponMods{ false };
	bool m_Misc_UnlimitedAmmo{ false };
	bool m_Misc_UnlimitedMagazine{ false };
	bool m_Misc_RapidFire{ false };
	int m_RapidFire_Value{};


	vec4 m_Visuals_Snaplines_Color{ 0.840f, 0.0f, 1.0f, 1.0f };
	vec4 m_BoundingBox_Color{ .x = 0.177f, .z = 1.0f, .w = 1.0f };
	vec4 m_FillBox_Color{ 0.910f, 0.039f, 1.0f, 0.199f };
	vec4 m_HealthBar_Color{ 0.0f, 1.0f, 0.818f, 1.0f };
	vec4 m_Name_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
};