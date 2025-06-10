#pragma once
#include "../../Game/entity.h"

class Visuals
{
public:
	void DrawLine(float ignore_outlined, float thickness, float x, float x2, float y, float y2, float w, vec4 color) const;
	void DrawRect(float ignore_outlined, float thickness, float x, float x2, float y, float y2, float w, vec4 color) const;
	void DrawFilledRect(float x, float x2, float y, float y2, float w, vec4 color) const;
	void Snaplines(const vec2& pos);
	void BoundingBox(const vec2& bottom, const vec2& top);
	void HealthBar(const Game::Entity& ent, const vec2& bottom, const vec2& top);
	void Render(const Game::Entity* entities);
public:
	float m_Matrix[16];
	bool m_Outlined;
	bool m_IgnoreTeammates;
	int m_Width;
	int m_Height;
};