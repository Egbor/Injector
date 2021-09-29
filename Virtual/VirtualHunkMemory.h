#pragma once

#include "VirtualPage.h"

class VirtualHunkMemory : public VirtualPage {
private:
	int mLowMark;
	int mSize;

public:
	VirtualHunkMemory(HANDLE hProcess, int size);
	~VirtualHunkMemory();

	void* Allocate(int size);
	void Free(void* space);

	void ForceRead(LPVOID lpBuffer, SIZE_T nSize) override;
	void ForceWrite(int nIndex, LPCVOID lpBuffer, SIZE_T nSize) override;

private:
	void AllocateVirtualSpace(int size);
	void FreeVirtualSpace();
};