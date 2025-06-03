#include "entity.h"
#include "../Memory/memory.hpp"
#include "../Globals/globals.h"
#include "offsets.hpp"
#include "../Math/math.hpp"

void Game::UpdateMyself(Entity& myself)
{
	Memory::rpm<Entity>(Globals::hProcess, Offsets::GetLocalPlayer(), myself);
	myself.dead = myself.health <= 0;
	myself.address = Offsets::GetLocalPlayer();
}

void Game::PopulateArray(std::array<uintptr_t, 31>& ents_ptr, Entity* ents)
{
	uintptr_t entity_list{ Offsets::GetEntityList() };
	unsigned int living_ents{ Offsets::GetLivingEntities() };

	/* get entity addresses */
	Memory::rpm_array<std::array<uintptr_t, 31>>(Globals::hProcess, entity_list, ents_ptr, sizeof(uintptr_t) * living_ents);

	for (unsigned int i{}; i < living_ents; ++i)
	{
		Memory::rpm<Entity>(Globals::hProcess, ents_ptr[i], ents[i]);
		ents[i].dead = ents[i].health <= 0;
		ents[i].address = ents_ptr[i];
	}
}

const Game::Entity* Game::ClosestEntity(const Entity* ents, const Entity& myself)
{
	const Entity* closest_entity{ &myself };
	float closest_distance{ static_cast<float>(0xBADF00D) };

	unsigned int living_ents{ Offsets::GetLivingEntities() };

	for (unsigned int i{ 0 }; i < living_ents; ++i)
	{
		if (ents[i].dead) continue;

		float distance{ Math::DistanceFrom(ents[i].feet, myself.feet) };
		if (distance < closest_distance)
		{
			closest_distance = distance;
			closest_entity = &ents[i];
		}
	}

	return closest_entity;
}