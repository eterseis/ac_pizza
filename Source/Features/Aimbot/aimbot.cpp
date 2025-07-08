#include "aimbot.h"
#include "../../Memory/memory.hpp"
#include "../../Globals/globals.h"
#include "../../Game/offsets.hpp"
#include "../../Math/math.hpp"
#include <cmath>

void Aimbot::ClosestTarget(const Game::Entity* ents, const Game::Entity& myself, bool enemyOnly)
{
	const Game::Entity* pTarget{ Game::ClosestEntity(ents, myself, enemyOnly) };
	if (pTarget->address == myself.address) return;

	constexpr float pi{ 3.14159265358979323846f };

	float abspos_x = pTarget->vHead.x - myself.vHead.x;
	float abspos_y = pTarget->vHead.y - myself.vHead.y;
	float abspos_z = pTarget->vHead.z - myself.vHead.z;

	float azimuth_xy = atan2f(abspos_y, abspos_x);
	float azimuth_z = atan2f(abspos_z, Math::DistanceFrom(pTarget->vHead, myself.vHead));

	float yaw = (azimuth_xy * (180.0f / pi));
	yaw += 90.0f;

	float pitch = (azimuth_z * (180.0f / pi));

	Memory::wpm<float>(Globals::hProcess, myself.address + Offsets::yaw, yaw);
	Memory::wpm<float>(Globals::hProcess, myself.address + Offsets::pitch, pitch);
}