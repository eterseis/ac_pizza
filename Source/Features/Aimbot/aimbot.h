#pragma once
#include "../../Game/game.h"

namespace Aimbot
{
	void ClosestTarget(const Game::Entity* ents, const Game::Entity& myself, bool enemyOnly);
}