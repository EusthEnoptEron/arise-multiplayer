#include "VMTHook.h"

LPVOID HookMethod(_In_ LPVOID lpVirtualTable, _In_ PVOID pHookMethod,
	_In_opt_ uintptr_t dwOffset)
{
	uintptr_t dwVTable = *((uintptr_t*)lpVirtualTable);
	uintptr_t dwEntry = dwVTable + dwOffset;
	uintptr_t dwOrig = *((uintptr_t*)dwEntry);

	DWORD dwOldProtection;
	::VirtualProtect((LPVOID)dwEntry, sizeof(dwEntry),
		PAGE_EXECUTE_READWRITE, &dwOldProtection);

	*((uintptr_t*)dwEntry) = (uintptr_t)pHookMethod;

	::VirtualProtect((LPVOID)dwEntry, sizeof(dwEntry),
		dwOldProtection, &dwOldProtection);

	return (LPVOID)dwOrig;
}