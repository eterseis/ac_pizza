#include "memory.hpp"

DWORD Memory::GetProcessId(const char* procName)
{
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	auto hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

	DWORD procId{};
	if (Process32First(hSnap, &procEntry))
	{
		do
		{
			if (!_stricmp(procName, procEntry.szExeFile))
			{

				procId = procEntry.th32ProcessID;
				break;
			}

		} while (Process32Next(hSnap, &procEntry));
	}

	CloseHandle(hSnap);
	return procId;
}

BYTE* Memory::GetModuleBaseAddress(const char* modName, DWORD procId = 0)
{
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(MODULEENTRY32);

	auto hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procId) };

	BYTE* baseAddr{ nullptr };
	if (Module32First(hSnap, &mEntry))
	{
		do
		{
			if (!_stricmp(modName, mEntry.szModule))
			{
				baseAddr = mEntry.modBaseAddr;
				break;
			}

		} while (Module32Next(hSnap, &mEntry));
	}

	CloseHandle(hSnap);
	return baseAddr;
}