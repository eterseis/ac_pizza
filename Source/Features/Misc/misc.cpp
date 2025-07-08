#include "misc.hpp"
#include "../../Memory/memory.hpp"
//#include "../../Game/game.h"
#include "../../Globals/globals.h"

void Misc::UnlimitedHealth()
{
	constexpr int newHealth{ 999 };
	Memory::wpm<int>(Globals::hProcess, reinterpret_cast<uintptr_t>(this->entity) + offsetof(Game::Entity, Game::Entity::health), newHealth);
}

void Misc::MaxArmor()
{
	constexpr int newArmor{ 100 };
	Memory::wpm<int>(Globals::hProcess, reinterpret_cast<uintptr_t>(this->entity) + offsetof(Game::Entity, Game::Entity::armor), newArmor);
}

void Misc::UnlimitedAmmo()
{
	constexpr int newAmmo{ 999 };
	Memory::wpm<int>(Globals::hProcess, reinterpret_cast<uintptr_t>(this->currentWeapon) + offsetof(Game::Weapon, Game::Weapon::ammo), newAmmo);
}

void Misc::UnlimitedMagazine()
{
	constexpr int newMagazine{ 999 };
	Memory::wpm<int>(Globals::hProcess, reinterpret_cast<uintptr_t>(this->currentWeapon) + offsetof(Game::Weapon, Game::Weapon::magazine), newMagazine);
}

void Misc::RapidFire(const int newFireRate)
{
	Memory::wpm<int>(Globals::hProcess, reinterpret_cast<uintptr_t>(this->currentWeapon) + offsetof(Game::Weapon, Game::Weapon::fireRate), newFireRate);
}