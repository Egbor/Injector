#include <Virutal/Virtual.h>
#include "VirtualReplacer.h"

#if defined(INJECTION_CODE)

#define OLD_STRING "This is a test string."
#define NEW_STRING "Replaced string!"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		VirtualReplaceString(OLD_STRING, NEW_STRING);
	}
	return TRUE;
}

#endif

void VirtualReplaceString(const char* oldString, const char* newString) {
	HANDLE hCurrentProcess = GetCurrentProcess();

	VirtualHunkMemory hunk(hCurrentProcess, MAXIMUM_HUNK_SIZE);
	VirtualScanner scanner(hCurrentProcess, hunk);
	VirtualReplacer replacer(hCurrentProcess, hunk);

	replacer.Replace(scanner, oldString, newString);
}