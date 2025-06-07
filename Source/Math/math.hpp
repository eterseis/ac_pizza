#pragma once
#include "../Game/vector.h"

namespace Math
{
	float DistanceFrom(const vec3& a, const vec3& b);
	bool WorldToScreen(const vec3& pos, vec2& screen, float* matrix, int width, int height);
}