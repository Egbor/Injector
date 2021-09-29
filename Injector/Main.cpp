#include <Windows.h>
#include <iostream>
#include <exception>

HANDLE OpenProcess(DWORD pid) {
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
	if (!hProcess) {
		throw std::exception("OpenProcess() failed: Immposible open process to inject.");
	}
	return hProcess;
}

int AllocateMemory(HANDLE hProcess, LPWSTR path, LPWSTR* lpRemoteBufferForLibraryPath) {
	int nBytesToAlloc = (1 + lstrlenW(path)) * sizeof(WCHAR);
	*lpRemoteBufferForLibraryPath = LPWSTR(VirtualAllocEx(hProcess, NULL, nBytesToAlloc, MEM_COMMIT, PAGE_READWRITE));
	if (!lpRemoteBufferForLibraryPath) {
		throw std::exception("AllocateMemory() failed: Immposible allocate virtual memory to inject.");
	}
	return nBytesToAlloc;
}

void InjectProcess(HANDLE hProcess, LPVOID lpAddress, LPCVOID lpBuffer, SIZE_T nSize) {
	if (!WriteProcessMemory(hProcess, lpAddress, lpBuffer, nSize, NULL)) {
		throw std::exception("InjectProcess() failed: Immposible write to the process memory.");
	}
}

void RunInjectedProcess(HANDLE hProcess, LPVOID lpAddress) {
	PTHREAD_START_ROUTINE pLoadLibraryFunction = reinterpret_cast<PTHREAD_START_ROUTINE>
		(GetProcAddress(GetModuleHandleW(L"Kernel32"), "LoadLibraryW"));
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, pLoadLibraryFunction, lpAddress, 0, NULL);
}

void Inject(DWORD pid, LPWSTR path) {
	int nBytesToAlloc;
	LPWSTR lpRemoteBufferForLibraryPath;
	HANDLE hProcess;

	//WCHAR path[1024];

	//GetCurrentDirectoryW(sizeof(path), path);
	//lstrcatW(path, L"\\Virtual.dll");

	hProcess = OpenProcess(pid);
	nBytesToAlloc = AllocateMemory(hProcess, path, &lpRemoteBufferForLibraryPath);

	InjectProcess(hProcess, lpRemoteBufferForLibraryPath, path, nBytesToAlloc);
	RunInjectedProcess(hProcess, lpRemoteBufferForLibraryPath);
}

int wmain(int argc, wchar_t** argv) {
	try {
		if (argc < 3) {
			throw std::exception("Injector faild: Not enought arguments.");
		}

		Inject(_wtoi(argv[1]), argv[2]);
	} catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}