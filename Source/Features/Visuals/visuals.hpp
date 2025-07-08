#pragma once
#include "../../Game/game.h"

class Visuals
{
public:
	void DrawLine(bool ignore_outlined, float thickness, float x, float x2, float y, float y2, float w, const vec4& color) const;
	void DrawRect(bool ignore_outlined, float thickness, float x, float x2, float y, float y2, float w, const vec4& color) const;
	void DrawFilledRect(float x, float x2, float y, float y2, float w, const vec4& color) const;
	void Snaplines(const vec2& pos);
	void BoundingBox(bool fill, const Game::Entity& ent, const vec2& bottom, const vec2& top);
	void HealthBar(const Game::Entity& ent, const vec2& bottom, const vec2& top, const Game::Entity& myself);
	void Name(const Game::Entity& ent, const vec2& bototm, const vec2& top, const vec2& text);
	void Render(const Game::Entity* entities, const Game::Entity& myself);
public:
	bool m_EnableVisuals;
	bool m_TeamCheck;
	bool m_HealthCheck;
	bool m_Outlined;

	bool m_Snaplines;
	bool m_BoundingBox;
	bool m_FillBox;
	bool m_HealthBar;
	bool m_Name;


	float m_Matrix[16];
	int m_Width;
	int m_Height;

	vec4 m_Snaplines_Color;
	vec4 m_BoundingBox_Color;
	vec4 m_FillBox_Color;
	vec4 m_HealthBar_Color;
	vec4 m_Name_Color;
};