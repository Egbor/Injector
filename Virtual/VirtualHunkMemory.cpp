#include "VirtualHunkMemory.h"
#include <exception>

VirtualHunkMemory::VirtualHunkMemory(HANDLE hProcess, int size) : VirtualPage(hProcess), mLowMark(0) {
	AllocateVirtualSpace(size);
}

VirtualHunkMemory::~VirtualHunkMemory() {
	FreeVirtualSpace();
}

void VirtualHunkMemory::AllocateVirtualSpace(int size) {
	MEMORYSTATUSEX memoryStatus;

	memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memoryStatus);

	if (memoryStatus.ullAvailVirtual < size) {
		throw std::exception("VirtualHunkMemory::AllocateVirtualSpace() failed: Not enought virtual memory.");
	}

	if (!(mAddress = VirtualAllocEx(mProcess, NULL, size, MEM_COMMIT, PAGE_READWRITE))) {
		throw std::exception("VirtualHunkMemory::AllocateVirtualSpace() failed: Immposible allocate virtual memory.");
	}
	mSize = size;
}

void VirtualHunkMemory::FreeVirtualSpace() {
	VirtualFreeEx(mProcess, mAddress, 0, MEM_RELEASE);
}

void* VirtualHunkMemory::Allocate(int size) {
	if ((mSize - mLowMark) < size) {
		throw std::exception("VirtualHunkMemory::Allocate() failed: Not enought memory.");
	}

	void* memory = reinterpret_cast<char*>(mAddress) + mLowMark;
	mLowMark += size;
	return memory;
}

void VirtualHunkMemory::Free(void* space) {
	mLowMark = reinterpret_cast<char*>(space) - reinterpret_cast<char*>(mAddress);
}

void VirtualHunkMemory::ForceRead(LPVOID lpBuffer, SIZE_T nSize) {
	throw std::exception("VirtualHunkMemory::ForceRead() failed: Hunk can not be read.");
}

void VirtualHunkMemory::ForceWrite(int nIndex, LPCVOID lpBuffer, SIZE_T nSize) {
	throw std::exception("VirtualHunkMemory::ForceWrite() failed: Hunk can not be written.");
}