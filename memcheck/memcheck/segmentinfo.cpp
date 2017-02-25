#include "segmentinfo.h"

ImageSectionInfo* GetSegmentInfo(PCHAR SegmentName)
{
	IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(GetModuleHandleA(0));
	IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);
	ImageSectionInfo *pSectionInfo = NULL;
	for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++)
	{
		char *name = (char*)pSectionHdr->Name;
		if (!strcmp(name, SegmentName))
		{
			pSectionInfo = new ImageSectionInfo(SegmentName);
			pSectionInfo->SectionAddress = (DWORD)GetModuleHandleA(0) + pSectionHdr->VirtualAddress;
			pSectionInfo->SectionSize = pSectionHdr->Misc.VirtualSize;
			return pSectionInfo;
		}
		pSectionHdr++;
	}
	return NULL;
}