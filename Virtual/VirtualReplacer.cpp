#include "VirtualReplacer.h"
#include <exception>

VirtualReplacer::VirtualReplacer(HANDLE hProcess, VirtualHunkMemory& hunk)
	: mCurrentProcess(hProcess), mHunk(&hunk), mReferenceString(), mPageDump() {
	AllocateReferenceString();
}

VirtualReplacer::~VirtualReplacer() {
	FreeReferenceString();
}

void VirtualReplacer::Replace(VirtualScanner& scanner, std::string oldString, std::string newString) {
	if (oldString.size() < newString.size()) {
		throw std::exception("VirtualReplacer::Replace failed: The length of the new string must be less than the length of the one.");
	}
	strcpy_s(mReferenceString.buffer, mReferenceString.size, oldString.c_str());

	VirtualPage* currentPage;
	while (currentPage = scanner.NextPage()) {
		if (currentPage->CheckProtect(0) || currentPage->CheckProtect(PAGE_NOACCESS)) {
			continue;
		}

		AllocatePageDump(currentPage->GetRegionSize());
		currentPage->ForceRead(mPageDump.buffer, mPageDump.size);

		for (int i = 0; i < mPageDump.size; i++) {
			if ((mPageDump.size - i) < mReferenceString.size) {
				break;
			}
			if (strcmp(mReferenceString.buffer, mPageDump.buffer + i) == 0) {
				currentPage->ForceWrite(i, newString.c_str(), newString.size() + 1);
				i += mReferenceString.size;
			}
		}

		FreePageDump();
	}
}

void VirtualReplacer::AllocatePageDump(int size) {
	mPageDump.buffer = reinterpret_cast<char*>(mHunk->Allocate(size));
	mPageDump.size = size;
}

void VirtualReplacer::AllocateReferenceString() {
	mReferenceString.buffer = reinterpret_cast<char*>(mHunk->Allocate(MAXIMUM_REFERENCE_SIZE));
	mReferenceString.size = MAXIMUM_REFERENCE_SIZE;
}

void VirtualReplacer::FreePageDump() {
	mHunk->Free(mPageDump.buffer);
	mPageDump.size = 0;
}

void VirtualReplacer::FreeReferenceString() {
	mHunk->Free(mReferenceString.buffer);
	mReferenceString.size = 0;
}