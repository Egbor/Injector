#include "VirtualScanner.h"
#include <exception>

VirtualScanner::VirtualScanner(HANDLE hProcess, VirtualHunkMemory& hunk) 
	: mProcess(hProcess), mHunk(&hunk), mCurrentPage(NULL) {
	Initialize();
}

VirtualScanner::~VirtualScanner() {
	mHunk->Free(mCurrentPage);
}

VirtualPage* VirtualScanner::Page(LPVOID lpAddress) {
	LPVOID lpBaseAddress = lpAddress;
	MEMORY_BASIC_INFORMATION memoryInformation;

	if (!VirtualQueryEx(mProcess, lpBaseAddress, &memoryInformation, sizeof(MEMORY_BASIC_INFORMATION))) {
		return NULL;
	}
	UpdateCurrentPage(lpBaseAddress, memoryInformation);

	return mCurrentPage;
}

VirtualPage* VirtualScanner::NextPage() {
	return Page(reinterpret_cast<char*>(mCurrentPage->GetBaseAddress()) + mCurrentPage->GetRegionSize());
}

void VirtualScanner::UpdateCurrentPage(LPVOID lpAddress, MEMORY_BASIC_INFORMATION information) {
	VirtualPage page(mProcess, lpAddress, information);

	if (mHunk->Equal(page)) {
		mCurrentPage = Page(reinterpret_cast<char*>(page.GetBaseAddress()) + page.GetRegionSize());
		return;
	}

	memcpy_s(mCurrentPage, sizeof(VirtualPage), &page, sizeof(VirtualPage));
}

void VirtualScanner::Initialize() {
	LPVOID lpBaseAddress = NULL;
	MEMORY_BASIC_INFORMATION memoryInfomation;

	mCurrentPage = reinterpret_cast<VirtualPage*>(mHunk->Allocate(sizeof(VirtualPage)));

	ZeroMemory(&memoryInfomation, sizeof(memoryInfomation));
	UpdateCurrentPage(lpBaseAddress, memoryInfomation);

	mCurrentPage = NextPage();
}