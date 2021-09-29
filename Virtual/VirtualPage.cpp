#include "VirtualPage.h"
#include <exception>

VirtualPage::VirtualPage(HANDLE hProcess)
	: mProcess(hProcess), mAddress(NULL), mInformation() {

}

VirtualPage::VirtualPage(HANDLE hProcess, LPVOID lpBaseAddress, MEMORY_BASIC_INFORMATION information) 
	: mProcess(hProcess), mAddress(lpBaseAddress), mInformation(information) {

}

void VirtualPage::ForceRead(LPVOID lpBuffer, SIZE_T nSize) {
	DWORD flOldProtect;

	VirtualProtectEx(mProcess, mAddress, mInformation.RegionSize, PAGE_EXECUTE_READWRITE, &flOldProtect);
	if (!ReadProcessMemory(mProcess, mAddress, lpBuffer, nSize, NULL)) {
		throw std::exception("VirtualPage::ForceRead() failed: Permission denied.");
	}
	VirtualProtectEx(mProcess, mAddress, mInformation.RegionSize, flOldProtect, NULL);
}

void VirtualPage::ForceWrite(int nIndex, LPCVOID lpBuffer, SIZE_T nSize) {
	if (mInformation.RegionSize < nIndex) {
		throw std::exception("VirtualPage::ForceWrite() failed: Out of page.");
	}

	DWORD flOldProtect;
	LPVOID lpAddressStart = reinterpret_cast<char*>(mAddress) + nIndex;

	VirtualProtectEx(mProcess, mAddress, mInformation.RegionSize, PAGE_EXECUTE_READWRITE, &flOldProtect);
	if (!WriteProcessMemory(mProcess, lpAddressStart, lpBuffer, nSize, NULL)) {
		throw std::exception("VirtualPage::ForceWrite() failed: Permission denied.");
	}
	VirtualProtectEx(mProcess, mAddress, mInformation.RegionSize, flOldProtect, NULL);
}

bool VirtualPage::Equal(VirtualPage& page) {
	return page.mAddress == mAddress;
}

bool VirtualPage::CheckProtect(int status) const {
	return mInformation.Protect == status;
}

int VirtualPage::GetRegionSize() const {
	return mInformation.RegionSize;
}

void* VirtualPage::GetBaseAddress() const {
	return mAddress;
}