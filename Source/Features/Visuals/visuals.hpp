#pragma once
#include "../../Game/entity.h"

class Visuals
{
public:
	void DrawLine(float ignore, float thickness, float x, float x2, float y, float y2, float w, vec4 color) const;
	void DrawRect(float ignore, float thickness, float x, float x2, float y, float y2, float w, vec4 color) const;
	void DrawFilledRect(float x, float x2, float y, float y2, float w, vec4 color) const;
	void TestViewMatrix(const Game::Entity* ents);
	void Snaplines(const Game::Entity& ent);
	void BoundingBox(const Game::Entity& ent);
	void HealthBar(const Game::Entity& ent);
	void RenderAll();
public:
	float m_Matrix[16];
	bool m_Outlined;
	bool m_IgnoreTeammates;
	int m_Width;
	int m_Height;
};