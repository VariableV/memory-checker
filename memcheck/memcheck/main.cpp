#include "segmentinfo.h"

#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <shlobj.h>

inline std::string GetAppDataPath()
{
	char buff[MAX_PATH];
	SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, 0, 0, buff);
	return buff;

}

// store a copy of the non tampered memory
void ScanSegment(int SegmentStart, int SegmentEnd)
{
	std::ofstream ofs;
	std::string buff("");
	
	ofs.open(GetAppDataPath() + "\\mem.dat");

	for (int i = SegmentStart; i < SegmentEnd; i++)
		if ((int)(*(char*)i) != -52 && (int)(*(char*)i)) // ignore instructions that are int3 or nothing of the integer representation
			buff += std::to_string((int)(*(char*)i)) + " "; // append the integer representation to the string

	ofs.write(buff.c_str(), buff.length()); // store the scanned seg
	ofs.close();
}

// compare program memory to the untampered version
void CompareMemory(int SegmentStart, int SegmentEnd)
{
	std::vector<int> mem;
	std::ifstream ifs;
	
	int temp, counter = 0;

	ifs.open(GetAppDataPath() + "\\mem.dat");
	while (ifs >> temp) // store the instructions for comparison
		mem.push_back(temp);
	ifs.close();

	for (int i = SegmentStart; i < SegmentEnd; i++)
		if ((int)*(char*)i != -52 && (int)*(char*)i)
		{
			if ((int)*(char*)i != mem.at(counter)) // memory check hit! user modified memory
				printf("memory change detected!\n");
			counter++;
		}
}

void StartMemoryLoop(int SegmentStart, int SegmentEnd)
{
	while (true)
	{
		CompareMemory(SegmentStart, SegmentEnd);
		Sleep(100);
	}
}

int main()
{
	ImageSectionInfo* text = GetSegmentInfo(".text");
	printf("%s 0x%08x 0x%08x\n", text->SectionName, text->SectionAddress, text->SectionAddress + text->SectionSize);
	printf("starting scan...\n");
	system("pause");

	ScanSegment(text->SectionAddress, text->SectionAddress + text->SectionSize);
	printf("completed, starting memory loop...\n");
	system("pause");

	std::thread t(StartMemoryLoop, text->SectionAddress, text->SectionAddress + text->SectionSize);
	t.detach();

	getchar();
	return 0;
}