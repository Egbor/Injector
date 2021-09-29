#pragma once

#include "VirtualHunkMemory.h"
#include "VirtualScanner.h"
#include <string>

class VirtualReplacer {
private:
	HANDLE mCurrentProcess;

	VirtualPageDump mPageDump;
	VirtualReferenceString mReferenceString;
	VirtualHunkMemory* mHunk;

public:
	VirtualReplacer(HANDLE hProcess, VirtualHunkMemory& hunk);
	~VirtualReplacer();

	void Replace(VirtualScanner& scanner, std::string oldString, std::string newString);

private:
	void AllocatePageDump(int size);
	void AllocateReferenceString();
	void FreePageDump();
	void FreeReferenceString();
};