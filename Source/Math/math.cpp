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