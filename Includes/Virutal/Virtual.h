#pragma once

#ifdef VIRTUAL_EXPORT
#define VIRTUAL_API __declspec(dllexport)
#else
#define VIRTUAL_API __declspec(dllimport)
#endif

#define MAXIMUM_HUNK_SIZE 0x00880000
#define MAXIMUM_REFERENCE_SIZE 0x00000400

typedef struct {
	char* buffer;
	int size;
} VirtualReferenceString, VirtualPageDump;

extern "C" VIRTUAL_API void VirtualReplaceString(const char* oldString, const char* newString);