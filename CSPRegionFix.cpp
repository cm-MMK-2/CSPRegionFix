// CSPRegionCrack.cpp : Entry Point
//

#include <string>
#include <iostream>
#include <fstream>

#define BYTE unsigned char

using namespace std;

BYTE* checkBytes;

BYTE* replaceBytes;

BYTE checkBytes64[] = { 0x75, 0x11, 0x3B, 0xF3, 0x74, 0x04, 0x3B, 0xC3, 0x75 };

BYTE replaceBytes64[] = { 0xEB, 0x11, 0x3B, 0xF3, 0x74, 0x04, 0x3B, 0xC3, 0x75 };

BYTE checkBytes32[] = { 0x75, 0x1B, 0x3B, 0xFE, 0x74, 0x04, 0x3B, 0xC6, 0x75 };

BYTE replaceBytes32[] = { 0xEB, 0x1B, 0x3B, 0xFE, 0x74, 0x04, 0x3B, 0xC6, 0x75 };

int GetIndexOfSubArray(const BYTE* mainArr, int mainArrSize,const BYTE* subArr, int subArrSize)
{
	for (int i = 0; i < mainArrSize - (subArrSize - 1); ++i)
	{
		if (mainArr[i] == subArr[0])
		{
			bool matches = true;
			for (unsigned int j = 1; j < subArrSize; ++j)
			{
				if (mainArr[i + j] != subArr[j])
				{
					matches = false;
					break;
				}
			}
			if (matches)
			{
				return i;
			}
		}
	}
	return -1;
}


void ReplaceSubArray(BYTE* mainArr, int startIndex, const BYTE* subArr, int subArrSize)
{
	for (int i = 0; i < subArrSize; ++i)
	{
		mainArr[startIndex + i] = subArr[i];
	}		
}

/*
Usage: 
64bit no arg
32bit -32
*/
int main(int argc, char* argv[]) {
	ifstream ifs("CLIPStudioPaint.exe", ios::binary);

	if (!ifs) {
		wcout << "Cannot find file -- CLIPStudioPaint.exe\nPress Any key to exit..." << endl;
		getchar();
		return -1;
	}

	if (argc > 0 && string(argv[1]).compare("-32"))
	{
		checkBytes = checkBytes32;
		replaceBytes = replaceBytes32;
		wcout << "Search 32bit data in file <CLIPStudioPaint.exe>..." << endl;
	}
	else
	{
		checkBytes = checkBytes64;
		replaceBytes = replaceBytes64;
		wcout << "Search 64bit data in file <CLIPStudioPaint.exe>..." << endl;
	}

	// get length of file:
	ifs.seekg(0, ifs.end);
	int length = ifs.tellg();
	ifs.seekg(0, ifs.beg);

	wcout << "Reading file... " << length << " bytes in total" << endl;

	BYTE* buffer = new BYTE[length];

	ifs.read((char *)buffer, length);

	if (ifs)
	{
		wcout << "Read success!" << endl;
	}
	else
	{
		wcout << "Error: Only " << ifs.gcount() << " bytes of data has been read." << endl;
		length = ifs.gcount();
	}
	ifs.close();

	int matchIndex = GetIndexOfSubArray(buffer, length, checkBytes, sizeof(checkBytes) / sizeof(BYTE));

	if (matchIndex != -1)
	{
		//backup
		ofstream bak_ofs("CLIPStudioPaint.exe.bak", ios::binary | ios::trunc);
		bak_ofs.write((char *)buffer, length);
		bak_ofs.close();
		wcout << "Cracking..." << endl;
		ReplaceSubArray(buffer, matchIndex, replaceBytes, sizeof(replaceBytes) / sizeof(BYTE));
		wcout << "Replace success..." << endl;
		ofstream ofs("CLIPStudioPaint.exe", ios::binary | ios::trunc);
		ofs.write((char *)buffer, length);
		ofs.close();
		wcout << "Region Fix success!" << endl;
	}
	else
	{
		wcout << "Cannot find matched data... Check if the file has already been cracked..." << endl;
		int matchCrackedIndex = GetIndexOfSubArray(buffer, length, replaceBytes, sizeof(replaceBytes) / sizeof(BYTE));
		if (matchCrackedIndex > 0)
		{
			wcout << "The file has already been fixed!" << endl;
		}
		else
		{
			wcout << "Cannot find target data block...Fix failed..." << endl;
		}
	}

	delete buffer;


	wcout << "Press any key to exit..." << endl;
	getchar();
	return 0;
}
