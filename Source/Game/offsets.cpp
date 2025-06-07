#include "offsets.hpp"
#include "../Memory/memory.hpp"
#include "../Globals/globals.h"

uintptr_t Offsets::GetEntityList()
{
	uintptr_t address{};
	Memory::rpm<uintptr_t>(Globals::hProcess, Offsets::entity_list + Globals::module_base, address);
	return address;
}

uintptr_t Offsets::GetLocalPlayer()
{
	uintptr_t address{};
	Memory::rpm<uintptr_t>(Globals::hProcess, Offsets::localplayer + Globals::module_base, address);
	return address;
}

unsigned int Offsets::GetLivingEntities()
{
	unsigned int alives{};
	Memory::rpm<unsigned int>(Globals::hProcess, Offsets::living_entities + Globals::module_base, alives);
	return (alives == 0) ? alives : alives - 1;
}

uintptr_t Offsets::GetViewMatrix()
{
	uintptr_t address{ Offsets::view_matrix + Globals::module_base - 0x2C };
	return address;
}