// http://stackoverflow.com/questions/4308996/finding-the-address-range-of-the-data-segment

#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

struct ImageSectionInfo
{
	char SectionName[IMAGE_SIZEOF_SHORT_NAME];
	DWORD SectionAddress;
	int SectionSize;

	ImageSectionInfo(const char* name)
	{
		strcpy(SectionName, name);
	}
};

ImageSectionInfo* GetSegmentInfo(PCHAR SegmentName);