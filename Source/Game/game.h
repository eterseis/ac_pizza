#pragma once

#include "vector.h"
#include <cstdint>
#include <array>

namespace Game
{
	struct Weapon
	{
		int magazine;
		char _padding[0x20];
		int ammo;
		char __padding[0x48 - 0x28];
		int fireRate;
		char ___padding[0x6C - 0x4C];
		int timesFired;
	};

	struct CurrentWeapon
	{
		uintptr_t vfTable;
		int id;
		uintptr_t weaponOwner;
		uintptr_t name;
		Weapon* weaponPtr;
	};

	struct Entity
	{
		uintptr_t vf_table;
		vec3 vHead;

		char _padding[0x28 - 0x10];
		vec3 vFeet;

		char __padding[0x44 - 0x34];
		float yaw;
		float pitch;

		char ___padding[0xEC - 0x4C];
		int health;
		int armor;

		char ____padding[0x205 - 0xF4];
		char name[16];

		char _____padding[0x30C - 0x215];
		int team;

		char ______padding[0x318 - 0x310];
		bool dead;

		char _______padding[0x364 - 0x31C];
		CurrentWeapon* currentWeaponPtr;
		/* end of the road */
		uintptr_t address;
	};

	void UpdateMyself(Entity& myself);
	void PopulateArray(uintptr_t* ents_ptr, Entity* ents);
	const Entity* ClosestEntity(const Entity* ents, const Entity& myself, bool enemyOnly);
}