#include "math.hpp"
#include <cmath>

float Math::DistanceFrom(const vec3& a, const vec3& b)
{
	vec3 point;
	point.x = (b.x - a.x) * (b.x - a.x);
	point.y = (b.y - a.y) * (b.y - a.y);
	point.z = (b.z - a.z) * (b.z - a.z);

	return std::sqrtf(point.x + point.y + point.z);
}

bool Math::WorldToScreen(const vec3& pos, vec2& screen, const float* matrix, const int width, const int height)
{
	vec4 convert;

	convert.x = (pos.x * matrix[0]) + (pos.y * matrix[4]) + (pos.z * matrix[8]) + matrix[12];
	convert.y = (pos.x * matrix[1]) + (pos.y * matrix[5]) + (pos.z * matrix[9]) + matrix[13];
	convert.z = (pos.x * matrix[2]) + (pos.y * matrix[6]) + (pos.z * matrix[10]) + matrix[14];
	convert.w = (pos.x * matrix[3]) + (pos.y * matrix[7]) + (pos.z * matrix[11]) + matrix[15];

	if (convert.w < 0.1f) return false; /*enemy is behind or out of window*/

	vec3 ndc;
	ndc.x = convert.x / convert.w;
	ndc.y = convert.y / convert.w;
	ndc.z = convert.z / convert.w;

	screen.x = (width / 2.0f * ndc.x) + (ndc.x + width / 2.0f);
	screen.y = (height / 2.0f * ndc.y) + (ndc.y + height / 2.0f);

	/* converts to a range of 1.0f && -1.0f */
	screen.x = (((screen.x - 0.0f) * (1.0f - -1.0f)) / (width - 0.0f)) + -1.0f;
	screen.y = (((screen.y - 0.0f) * (1.0f - -1.0f)) / (height - 0.0f)) + -1.0f;

	return true;
}