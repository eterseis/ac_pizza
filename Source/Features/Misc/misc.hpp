#pragma once
#include "../../Game/game.h"
#include <cstdint>

class Misc
{
public:
	void UnlimitedHealth();
	void MaxArmor();
	void UnlimitedAmmo();
	void UnlimitedMagazine();
	void RapidFire(const int newFireRate);

public:
	Game::Entity* entity;
	Game::Weapon* currentWeapon;
};