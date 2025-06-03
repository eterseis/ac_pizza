#include "entity.h"
#include "../Memory/memory.hpp"
#include "../Globals/globals.h"
#include "offsets.hpp"

void Game::populate_array(std::array<uintptr_t, 31>& ents_ptr, Entity* ents)
{
	uintptr_t entity_list{ Offsets::GetEntityList() };
	unsigned int living_ents{ Offsets::GetLivingEntities() - 1 };

	/* get entity addresses */
	Memory::rpm_array<std::array<uintptr_t, 31>>(Globals::hProcess, entity_list, ents_ptr, sizeof(uintptr_t) * living_ents);

	for (unsigned int i{}; i < living_ents; ++i)
	{
		Memory::rpm<Entity>(Globals::hProcess, ents_ptr[i], ents[i]);
		ents[i].dead = ents[i].health < 0;
		ents[i].address = ents_ptr[i];
	}
}