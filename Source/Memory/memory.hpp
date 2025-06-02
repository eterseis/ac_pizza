#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

namespace Memory
{
	DWORD GetProcessId(const char* proc);
	BYTE* GetModuleBaseAddress(const char* proc, DWORD id);

	template <typename T>
	void rpm(HANDLE hProc, uintptr_t address, T& buffer)
	{
		::ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(buffer), nullptr);
	}

	template <typename T>
	void wpm(HANDLE hProc, uintptr_t address, T& buffer)
	{
		::WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(address), &buffer, sizeof(buffer), nullptr);
	}
}