#include "pch.hpp"
#include "Memory/memory.hpp"
#include "Game/entity.h"
#include <vector>

int main()
{
	auto procId = Memory::GetProcessId("ac_client.exe");
	Game::module_base = reinterpret_cast<uintptr_t>(Memory::GetModuleBaseAddress("ac_client.exe", procId));
	Game::hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, procId);

	if (!Game::hProcess)
	{
		std::cout << "failed to open process\n";
		return 1;
	}

	std::array<uintptr_t, 31> entities_ptr;
	std::vector<Game::Entity> entities;
	entities.resize(31);

	Game::populate_array(entities_ptr, entities);

	CloseHandle(Game::hProcess);
	return 0;
}