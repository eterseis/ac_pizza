#pragma once

#include "vector.h"
#include <cstdint>
#include <array>

namespace Game
{
	struct Entity
	{
		uintptr_t vf_table;
		vec3 vHead;

		char padding[0x28 - 0x10];
		vec3 vFeet;

		char padding2[0x44 - 0x34];
		float yaw;
		float pitch;

		char padding3[0xEC - 0x4C];
		int health;

		char padding4[0x205 - 0xF0];
		char name[16];

		char padding5[0x30C - 0x215];
		int team;
		bool dead;
		uintptr_t address;
	};

	void UpdateMyself(Entity& myself);
	void PopulateArray(uintptr_t* ents_ptr, Entity* ents);
	const Entity* ClosestEntity(const Entity* ents, const Entity& myself);
}