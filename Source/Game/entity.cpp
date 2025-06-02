#include "entity.h"
#include "../Memory/memory.hpp"
#include "offsets.hpp"

void Game::populate_array(std::array<uintptr_t, 31>& ents_ptr, std::vector<Entity>& ents)
{
	uintptr_t entity_list;
	Memory::rpm<uintptr_t>(Game::hProcess, Offsets::entity_list + Game::module_base, entity_list);

	Memory::rpm<std::array<uintptr_t, 31>>(Game::hProcess, entity_list, ents_ptr);

	int living_ents;
	Memory::rpm<int>(Game::hProcess, Offsets::living_entities + Game::module_base, living_ents);

	for (int i{}; i < living_ents - 1; ++i)
	{
		Memory::rpm<Entity>(Game::hProcess, ents_ptr[i], ents[i]);
	}
}