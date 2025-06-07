#include "visuals.hpp"
#include "../../Math/math.hpp"
#include "../../Game/offsets.hpp"

#include "../../Dependencies/GLEW/glew.h"
#include "../../Dependencies/GLFW/glfw3.h"

void Visuals::DrawLine(float ignore, float thickness, float x, float x2, float y, float y2, float w, vec4 color) const
{
	if (this->m_Outlined && !ignore)
	{
		Visuals::DrawLine(true, thickness + 1.0f, x, x2, y, y2, w, { 0.0f, 0.0f, 0.0f, 1.0f });
	}

	glColor4f(color.x, color.y, color.z, color.w);
	glLineWidth(thickness);
	glBegin(GL_LINES);

	glVertex2f(x - w, y);
	glVertex2f(x2 - w, y2);

	glEnd();
}

void Visuals::DrawRect(float ignore, float thickness, float x, float x2, float y, float y2, float w, vec4 color) const
{
	if (this->m_Outlined && !ignore)
	{
		Visuals::DrawRect(true, thickness + 1.0f, x, x2, y, y2, w, { 0.0f, 0.0f, 0.0f, 1.0f });
	}

	glColor4f(color.x, color.y, color.z, color.w);
	glLineWidth(thickness);
	glBegin(GL_LINES);

	glVertex2f(x - w, y);
	glVertex2f(x + w, y);

	glVertex2f(x2 - w, y2);
	glVertex2f(x2 + w, y2);

	glVertex2f(x - w, y);
	glVertex2f(x2 - w, y2);

	glVertex2f(x + w, y);
	glVertex2f(x2 + w, y2);

	glEnd();
}

void Visuals::DrawFilledRect(float x, float x2, float y, float y2, float w, vec4 color) const
{
	glColor4f(color.x, color.y, color.z, color.w);
	glBegin(GL_TRIANGLES);

	glVertex2f(x - w, y);
	glVertex2f(x + w, y);
	glVertex2f(x2 + w, y2);

	glVertex2f(x2 - w, y2);
	glVertex2f(x2 + w, y2);
	glVertex2f(x - w, y);

	glEnd();
}

void Visuals::TestViewMatrix(const Game::Entity* ents)
{
	const auto living_ents{ Offsets::GetLivingEntities() };

	for (unsigned int i{ 0 }; i < living_ents; ++i)
	{
		Visuals::Snaplines(ents[i]);
		Visuals::BoundingBox(ents[i]);
		Visuals::HealthBar(ents[i]);
	}
}

void Visuals::Snaplines(const Game::Entity& ent)
{
	vec2 screen;
	if (!Math::WorldToScreen(ent.vFeet, screen, this->m_Matrix, this->m_Width, this->m_Height)) return;

	constexpr vec2 origin{ 0.0f, -1.0f };
	Visuals::DrawLine(false, 1.0f, screen.x, origin.x, screen.y, origin.y, 0.0f, vec4{ 1.0f, 0.0f, 0.0f, 1.f });
}

void Visuals::BoundingBox(const Game::Entity& ent)
{
	vec2 bottom;
	if (!Math::WorldToScreen(ent.vFeet, bottom, this->m_Matrix, this->m_Width, this->m_Height)) return;

	vec2 top;
	if (!Math::WorldToScreen({ ent.vHead.x, ent.vHead.y, ent.vHead.z + 0.8f }, top, this->m_Matrix, this->m_Width, this->m_Height)) return;

	float height{ top.y - bottom.y };
	float width{ height / 5.0f };

	Visuals::DrawRect(false, 1.0f, bottom.x, top.x, bottom.y, top.y, width, vec4{ 1.f, .0f, .0f, 1.0f });
	Visuals::DrawFilledRect(bottom.x, top.x, bottom.y, top.y, width, vec4{ 1.f, 0.f, 0.f, 0.2f });
}

void Visuals::HealthBar(const Game::Entity& ent)
{
	vec2 bottom;
	if (!Math::WorldToScreen(ent.vFeet, bottom, this->m_Matrix, this->m_Width, this->m_Height)) return;

	vec2 top;
	if (!Math::WorldToScreen({ ent.vHead.x, ent.vHead.y, ent.vHead.z + 0.8f }, top, this->m_Matrix, this->m_Width, this->m_Height)) return;


	float height{ top.y - bottom.y };
	float width{ height / 4.0f };

	/* Linear interpolation with percentage */
	float t{ 1.0f - (ent.health / 100.0f) };
	(t > 1.0f) ? t = 1.0f : 1;
	float x{ top.x + t * (bottom.x - top.x) };
	float y{ top.y + t * (bottom.y - top.y) };

	if (this->m_Outlined)
	{
		Visuals::DrawLine(true, 2.5f, bottom.x, top.x, bottom.y, top.y, width, { 0.0f, 0.0f, 0.0f, 1.0f });
	}

	Visuals::DrawLine(true, 1.0f, x, bottom.x, y, bottom.y, width, { .0f, 1.f, .0f, 1.f });
}

void Visuals::RenderAll()
{
	const auto living_ents{ Offsets::GetLivingEntities() };
	for (unsigned int i{ 0 }; i < living_ents; ++i)
	{

	}
}