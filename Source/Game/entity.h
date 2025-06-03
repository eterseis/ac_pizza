#pragma once

#include "vector.h"
#include <cstdint>
#include <array>

namespace Game
{
	struct Entity
	{
		uintptr_t vf_table;
		vec3 head;

		char padding[0x28 - 0x10];
		vec3 feet;

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

	void populate_array(std::array<uintptr_t, 31>& ents_ptr, Entity* ents);
}