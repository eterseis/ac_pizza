#include "entity.h"
#include "../Memory/memory.hpp"
#include "../Globals/globals.h"
#include "offsets.hpp"
#include "../Math/math.hpp"

void Game::UpdateMyself(Entity& myself)
{
	uintptr_t address{ Offsets::GetLocalPlayer() };

	Memory::rpm<Entity>(Globals::hProcess, address, myself, 784);
	myself.dead = myself.health <= 0;
	myself.address = address;
}

void Game::PopulateArray(uintptr_t* ents_ptr, Entity* ents)
{
	uintptr_t entity_list{ Offsets::GetEntityList() };
	unsigned int living_ents{ Offsets::GetLivingEntities() };

	/* get entity addresses */
	Memory::rpm_array(Globals::hProcess, entity_list, ents_ptr, sizeof(uintptr_t) * living_ents);
	//Memory::rpm_array <

	for (unsigned int i{}; i < living_ents; ++i)
	{
		Memory::rpm<Game::Entity>(Globals::hProcess, ents_ptr[i], ents[i], 784);
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

		float distance{ Math::DistanceFrom(ents[i].vFeet, myself.vFeet) };
		if (distance < closest_distance)
		{
			closest_distance = distance;
			closest_entity = &ents[i];
		}
	}

	return closest_entity;
}