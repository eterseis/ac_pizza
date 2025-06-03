#include "pch.hpp"
#include "Memory/memory.hpp"
#include "Game/entity.h"
#include "Globals/globals.h"
#include "Features/Aimbot/aimbot.h"

class Timer
{
public:
	Timer()
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		Stop();
	}
	void Stop()
	{
		auto endTimePoint{ std::chrono::high_resolution_clock::now() };

		auto start{ std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count() };
		auto end{ std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count() };

		auto duration{ end - start };
		double ms{ duration * 0.001 };

		std::cout << duration << "us (" << ms << "ms)\n";
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
};

int main()
{
	auto procId = Memory::GetProcessId("ac_client.exe");
	Globals::module_base = reinterpret_cast<uintptr_t>(Memory::GetModuleBaseAddress("ac_client.exe", procId));
	Globals::hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, procId);

	if (!Globals::hProcess)
	{
		std::cout << "failed to find assault cube process\n";
		return -1;
	}

	std::array<uintptr_t, 31> entityAddresses;
	Game::Entity* entities = new Game::Entity[31];
	Game::Entity myself;

	while (true)
	{
		Game::UpdateMyself(myself);
		Game::PopulateArray(entityAddresses, entities);
		const Game::Entity* blup{ Game::ClosestEntity(entities, myself) };
		Aimbot::ClosestTarget(*blup, myself);
	}


	delete[] entities;
	CloseHandle(Globals::hProcess);
	return 0;
}