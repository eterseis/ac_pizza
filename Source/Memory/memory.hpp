#pragma once

#define WIN32_LEAN_AND_MEAN
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
	void rpm_array(HANDLE hProc, uintptr_t address, T& buffer, SIZE_T size)
	{
		::ReadProcessMemory(hProc, reinterpret_cast<LPCVOID>(address), &buffer, size, nullptr);
	}

	template <typename T>
	void wpm(HANDLE hProc, uintptr_t address, T& buffer)
	{
		::WriteProcessMemory(hProc, reinterpret_cast<LPVOID>(address), &buffer, sizeof(buffer), nullptr);
	}
}