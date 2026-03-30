//#include <iostream>
// Shagg_E, 2016-2026.
//
#include "stdafx.h"
#include <windows.h>
#include <conio.h>

bool AreTwoCharsIdentical(const char *char1, const char *char2)
{
	int identical = true;
	int char1Length = (unsigned)strlen(char1); // char1 length
	int char2Length = (unsigned)strlen(char2); // char2 length
	int char1Check;
	if (char1Length == char2Length) { // if chars Lengths are identical
		for (int i = 0; i < char1Length; i++) { // check every char
			if (!(char1[i] == char2[i])) { // if chars are not identical
				char1Check = char1[i]; // maybe they has different registers
				if (char1Check > 64 && char1Check < 91) { // if char1Check has High register
					char1Check = char1Check + 32; // register down
					if (!(char1Check == char2[i])) { // check again
						identical = false; // nope
					}
				}
				else if (char1Check > 96 && char1Check < 123) { // if char1Check has Down register
					char1Check = char1Check - 32; // register up
					if (!(char1Check == char2[i])) { // check again
						identical = false; // nope
					}
				}
				else {
					identical = false; // nope
				}
			}
		}
	}
	else {
		identical = false; // nope
	}
	return identical;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int FilesFoundCount = 0;
	int FilesMovedCount = 0;
	int ErrorsCount = 0;
	int ExtensionsCount = 0;
	int FileNameLength;
	int ExtensionPointer;
	char SortOutFolder[255];
	char SortOutFolder1[255];
	char SortOutFolder2[255];
	char SortOutFolder3[255];
	char FindOneExtension[255];
	char CreateLogFile[255];
	char LogString[255];
	char LogString2[255];
	char LogString3[255];
	char FilePath1[255];
	char FilePath2[255];
	char FileFullName[255];
	char FileNameExtension[255];
	char FileNameExtensionFolder[255];
	GetPrivateProfileStringA("SETTINGS", "SortOutFolder", "0", SortOutFolder, 255, (LPCSTR)".\\SortThisOut.ini");
	GetPrivateProfileStringA("SETTINGS", "FindOneExtension", "0", FindOneExtension, 255, (LPCSTR)".\\SortThisOut.ini");
	GetPrivateProfileStringA("SETTINGS", "CreateLogFile", "0", CreateLogFile, 255, (LPCSTR)".\\SortThisOut.ini");
	if (CreateLogFile[0] != 48)	WritePrivateProfileStructA("LOG", NULL, NULL, 0, (".\\SortThisOut.log")); // Clear Log
	WIN32_FIND_DATAA fd;
	//printf("Test1..\n");
	if ((SortOutFolder[0] == 0) || (SortOutFolder[0] == 48))
	{
		sprintf_s(SortOutFolder1, "*.*");
		sprintf_s(SortOutFolder2, ".");
	}
	else
	{
		sprintf_s(SortOutFolder1, "%s\\*.*", SortOutFolder);
		sprintf_s(SortOutFolder2, "./%s", SortOutFolder);
		HANDLE hFindsof = FindFirstFileA(SortOutFolder1, &fd);
		if (hFindsof == INVALID_HANDLE_VALUE)
		{
			sprintf_s(LogString, "Folder '%s' is not exists!", SortOutFolder);
			if (CreateLogFile[0] != 48)	WritePrivateProfileStringA("LOG", "ERROR", LogString, (LPCSTR)".\\SortThisOut.log");
			printf("ERROR: %s\n", LogString);
			ErrorsCount++;
		}
		FindClose(hFindsof);
	}
	if (ErrorsCount == 0)
	{
		HANDLE hFind = FindFirstFileA(SortOutFolder1, &fd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				//if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(AreTwoCharsIdentical(fd.cFileName, "SortThisOut.exe")) && !(AreTwoCharsIdentical(fd.cFileName, "SortThisOut.log")) && !(AreTwoCharsIdentical(fd.cFileName, "SortThisOut.ini")))
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(AreTwoCharsIdentical(fd.cFileName, "SortThisOut.exe")) && !(AreTwoCharsIdentical(fd.cFileName, "SortThisOut2.exe")) && !(AreTwoCharsIdentical(fd.cFileName, "SortThisOut.log")) && !(AreTwoCharsIdentical(fd.cFileName, "SortThisOut.ini")))
				{
					FilesFoundCount++;
					FileNameLength = 0;
					ExtensionPointer = 0;
					FileNameLength = (unsigned)strlen(fd.cFileName);
					for (int i = FileNameLength; i > 0; i--) // finding extension dot
					{
						if (fd.cFileName[i] == 46)
						{
							ExtensionPointer = i + 1;
							break;
						}
					}
					if (ExtensionPointer > 0)	sprintf_s(FileNameExtension, "%s", fd.cFileName + ExtensionPointer);
					else	sprintf_s(FileNameExtension, "NO EXTENSION");
					sprintf_s(FileFullName, "%s", fd.cFileName);

					if (((FindOneExtension[0] != 0) && (FindOneExtension[0] != 48) && (AreTwoCharsIdentical(FileNameExtension, FindOneExtension))) || (FindOneExtension[0] == 0) || (FindOneExtension[0] == 48))
					{
						if ((SortOutFolder[0] == 0) || (SortOutFolder[0] == 48))
						{
							sprintf_s(SortOutFolder3, "%s\\*.*", FileNameExtension);
							if ((SortOutFolder[0] == 0) || (SortOutFolder[0] == 48))	sprintf_s(FileNameExtensionFolder, "%s", FileNameExtension);
						}
						else
						{
							sprintf_s(SortOutFolder3, "%s\\%s\\*.*", SortOutFolder, FileNameExtension);
							sprintf_s(FileNameExtensionFolder, "%s\\%s", SortOutFolder, FileNameExtension);
						}
						HANDLE hFindef = FindFirstFileA(SortOutFolder3, &fd);
						if (hFindef == INVALID_HANDLE_VALUE)
						{
							CreateDirectoryA(FileNameExtensionFolder, NULL);
							ExtensionsCount++;
						}
						FindClose(hFindef);
						sprintf_s(LogString, "%d", FilesFoundCount);
						sprintf_s(FilePath1, "%s/%s", SortOutFolder2, FileFullName);
						sprintf_s(FilePath2, "%s/%s/%s", SortOutFolder2, FileNameExtension, FileFullName);
						if (MoveFileA(FilePath1, FilePath2))
						{
							sprintf_s(LogString2, "%s found and moved to '%s' folder", FileFullName, FileNameExtensionFolder);
							FilesMovedCount++;
						}
						else
						{
							sprintf_s(LogString2, "ERROR: %s can't move to '%s' folder", FileFullName, FileNameExtensionFolder);
							ErrorsCount++;
						}
						if (CreateLogFile[0] != 48)	WritePrivateProfileStringA("LOG", LogString, LogString2, (LPCSTR)".\\SortThisOut.log");
						printf("%s: %s\n", LogString, LogString2);
					}
				}
				//else printf("Can't find any shit...\n");
			} while (FindNextFileA(hFind, &fd));
			FindClose(hFind);
		}
	}
	//else printf("Errors? Wtf is this shit?..\n");
	sprintf_s(LogString, "%d files. Folders created for extensions: %d", FilesFoundCount, ExtensionsCount);
	sprintf_s(LogString2, "%d files", FilesMovedCount);
	sprintf_s(LogString3, "%d", ErrorsCount);
	if (CreateLogFile[0] != 48)
	{
		WritePrivateProfileStringA("LOG", "Total Found", LogString, (LPCSTR)".\\SortThisOut.log");
		WritePrivateProfileStringA("LOG", "Total Moved", LogString2, (LPCSTR)".\\SortThisOut.log");
		WritePrivateProfileStringA("LOG", "Total Errors", LogString3, (LPCSTR)".\\SortThisOut.log");
	}
	printf("Total Found: %s\n", LogString);
	printf("Total Moved: %s\n", LogString2);
	printf("Total Errors: %s\n", LogString3);
	printf("Press any key to exit");
	_getch();
	return 0;
}