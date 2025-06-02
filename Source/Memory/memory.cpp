#include "memory.hpp"

DWORD Memory::GetProcessId(const char* proc)
{
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	auto snap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

	if (Process32First(snap, &procEntry))
	{
		do
		{
			if (!_stricmp(proc, procEntry.szExeFile))
			{
				break;
			}

		} while (Process32Next(snap, &procEntry));
	}

	CloseHandle(snap);
	return procEntry.th32ProcessID;
}

BYTE* Memory::GetModuleBaseAddress(const char* proc, DWORD id = 0)
{
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(MODULEENTRY32);

	auto snap{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id) };

	if (Module32First(snap, &mEntry))
	{
		do
		{
			if (!_stricmp(proc, mEntry.szModule))
			{
				break;
			}

		} while (Module32Next(snap, &mEntry));
	}

	CloseHandle(snap);
	return mEntry.modBaseAddr;
}