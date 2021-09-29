#pragma once

#include <Virutal/Virtual.h>
#include <Windows.h>

class VirtualPage {
protected:
	HANDLE mProcess;
	LPVOID mAddress;

private:
	MEMORY_BASIC_INFORMATION mInformation;

public:
	VirtualPage(HANDLE hProcess);
	VirtualPage(HANDLE hProcess, LPVOID lpBaseAddress, MEMORY_BASIC_INFORMATION information);
	~VirtualPage() = default;

	virtual void ForceRead(LPVOID lpBuffer, SIZE_T nSize);
	virtual void ForceWrite(int nIndex, LPCVOID lpBuffer, SIZE_T nSize);

	bool Equal(VirtualPage& page);
	bool CheckProtect(int status) const;

	int GetRegionSize() const;
	void* GetBaseAddress() const;
};