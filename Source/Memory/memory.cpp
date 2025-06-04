#include "memory.hpp"

DWORD Memory::GetProcessId(const char* proc)
{
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);
	auto hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

	if (Process32First(hSnap, &procEntry))
	{
		do
		{
			if (!_stricmp(proc, procEntry.szExeFile))
			{
				CloseHandle(hSnap);
				return procEntry.th32ProcessID;
			}

		} while (Process32Next(hSnap, &procEntry));
	}

	CloseHandle(hSnap);
	return 0;
}

BYTE* Memory::GetModuleBaseAddress(const char* proc, DWORD id = 0)
{
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(MODULEENTRY32);

	auto hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id) };

	if (Module32First(hSnap, &mEntry))
	{
		do
		{
			if (!_stricmp(proc, mEntry.szModule))
			{
				CloseHandle(hSnap);
				return mEntry.modBaseAddr;
			}

		} while (Module32Next(hSnap, &mEntry));
	}

	CloseHandle(hSnap);
	return 0;
}