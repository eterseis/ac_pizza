#pragma once
#include "../../Game/entity.h"

class Visuals
{
public:
	void DrawLine(bool ignore_outlined, float thickness, float x, float x2, float y, float y2, float w, const vec4& color) const;
	void DrawRect(bool ignore_outlined, float thickness, float x, float x2, float y, float y2, float w, const vec4& color) const;
	void DrawFilledRect(float x, float x2, float y, float y2, float w, const vec4& color) const;
	void Snaplines(const vec2& pos);
	void BoundingBox(bool fill, const vec2& bottom, const vec2& top);
	void HealthBar(const Game::Entity& ent, const vec2& bottom, const vec2& top);
	void Render(const Game::Entity* entities, const Game::Entity& myself);
public:
	float m_Matrix[16];

	bool m_EnableVisuals;
	bool m_TeamCheck;
	bool m_HealthCheck;
	bool m_Outlined;

	bool m_Snaplines;
	bool m_BoundingBox;
	bool m_FillBox;

	bool m_HealthBar;

	vec4 m_Snaplines_Color;
	vec4 m_BoundingBox_Color;
	vec4 m_FillBox_Color;
	vec4 m_HealthBar_Color;

	int m_Width;
	int m_Height;
};