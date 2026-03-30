// This tool was made by Shagg_E, 2017.
// Original decryption algorithm was made by unknown author.
//
#include <stdio.h>
#include "stdafx.h"
#include <windows.h>
#include <conio.h>

FILE *stream, *stream2;

bool IsTwoCHARsAreIdentical(char Char1[], char Char2[])
{
	bool Identical = true; // "yes" by default
	int Char1Length = (unsigned)strlen(Char1); // Char1 length
	int Char2Length = (unsigned)strlen(Char2); // Char2 length
	int Char1Check;
	if(Char1Length==Char2Length) // if CHARs Lengths are identical
	{
		for (int i=0;i<Char1Length;i++) // check every char
		{
			if(!(Char1[i]==Char2[i])) // if chars are not identical
			{
				Char1Check=Char1[i]; // maybe they has different registers
				if(Char1Check>64 && Char1Check<91) // if Char1Check has High register
				{
					Char1Check=Char1Check+32; // register down
					if(!(Char1Check==Char2[i])) // check again
					{
						Identical = false; // "nope"
					}
				}
				else if(Char1Check>96 && Char1Check<123) // if Char1Check has Down register
				{
					Char1Check=Char1Check-32; // register up
					if(!(Char1Check==Char2[i])) // check again
					{
						Identical = false; // "nope"
					}
				}
				else
				{
					Identical = false; // "nope"
				}
			}

		}
	}
	else
	{
		Identical = false; // "nope"
	}
	return Identical;
}

int key = 0xECD6D157;
unsigned short DoNumber() 
{
    unsigned long x,y;
    x = key;
    y = x + x * 2;
    y = x + y * 4;
    y = y * 0x10;
    y += x;
    y = y * 0x100;
    y -= x;
    x = x + y * 4 + 0x269EC3;
    key = x;
    __asm mov eax,x;
    __asm sar eax,0x10;
    __asm mov x,eax;
    return x & 0x7fff;
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf ("Welcome to MAXScript Decrypter\n\n");
	printf ("Scanning folder for .mse scripts...\n\n");
	int FileNameLength;
	int ExtensionPointer;
	int FilesFoundCount = 0;
	int FilesDecryptedCount = 0;
	int ErrorsCount = 0;
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA("*.*", &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				FileNameLength = 0;
				ExtensionPointer = 0;
				FileNameLength = (unsigned)strlen(fd.cFileName);
				for (int i=FileNameLength;i>0;i--) // finding extension dot
				{
					if(fd.cFileName[i]==46)
					{
						ExtensionPointer=i+1;
						break;
					}
				}
				if(ExtensionPointer>0)
				{
					char FileNameExtension[255];
					sprintf_s(FileNameExtension, "%s", fd.cFileName+ExtensionPointer);
					if(IsTwoCHARsAreIdentical(FileNameExtension, "mse"))
					{
						FilesFoundCount++;
						key = 0xECD6D157;
						char LogString[255];
						errno_t f1 = fopen_s(&stream, fd.cFileName,"rb");
						if( f1 == 0 )
						{
							char NewFileName[255];
							strncpy_s(NewFileName, fd.cFileName, ExtensionPointer-1); // from name begin to char before extension
							sprintf_s(NewFileName, "%sDecrypted.ms", NewFileName);
							errno_t f2 = fopen_s(&stream2, NewFileName,"wb");
							if( f2 == 0 )
							{
								short num;
								char c;
								while (!feof(stream)) 
								{
									c = fgetc(stream);
									num = DoNumber();
									c ^= num;
									fputc(c,stream2);
								}
								f2 = fclose(stream2);
								sprintf_s(LogString, "%s was decrypted and saved as %s\n", fd.cFileName, NewFileName);
								FilesDecryptedCount++;
							}
							else
							{
								sprintf_s(LogString, "ERROR: Can't save file %s\n", NewFileName);
								ErrorsCount++;
							}
							f1 = fclose(stream);
						}
						else
						{
							sprintf_s(LogString, "ERROR: Can't open file %s\n", fd.cFileName);
							ErrorsCount++;
						}
						printf (LogString);
					}
				}
			}

		}
		while(FindNextFileA(hFind, &fd));
		FindClose(hFind);
	}
	printf ("\nFound: %d scripts\n", FilesFoundCount);
	printf ("Decrypted: %d scripts\n", FilesDecryptedCount);
	printf ("Errors: %d\n\n", ErrorsCount);
	printf ("Press any key to exit");
	_getch();
	return 0;
}