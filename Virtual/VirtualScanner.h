#pragma once

#include "VirtualHunkMemory.h"

class VirtualScanner {
private:
	HANDLE mProcess;

	VirtualHunkMemory* mHunk;
	VirtualPage* mCurrentPage;

public:
	VirtualScanner(HANDLE hProcess, VirtualHunkMemory& hunk);
	~VirtualScanner();

	VirtualPage* Page(LPVOID lpAddress);
	VirtualPage* NextPage();

private:
	void Initialize();
	void UpdateCurrentPage(LPVOID lpAddress, MEMORY_BASIC_INFORMATION information);
};