// ConsoleApplication4.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <shellapi.h>
using namespace std;
int main()
{
	void CreateFlag();
	CreateFlag();
	HANDLE hMutex = CreateMutex(0, 1, L"Fuck");
	DWORD Flags = GetLastError();
	if (hMutex != 0)
	{
		if (Flags == ERROR_ALREADY_EXISTS)
		{
			MessageBox(0, L"This Program is Running", L"Fuck", MB_OK);
			exit(0);
		}
		DeleteFile(L"Debugger.ini");
		exit(0);
	}
    else { MessageBox(0, L"Mutex Creat Faile", L"Fuck", MB_OK); }

	DeleteFile(L"Debugger.ini");


}
void CreateFlag()
{
	HANDLE hFile = CreateFile(L"Debugger.ini", GENERIC_WRITE, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_HIDDEN, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(0, L"This Program is Running", L"Fuck", MB_OK);
		DeleteFile(L"Debugger.ini");
		exit(0);
	}
	char chBuffer[4096] = { 0x4D };
	DWORD dwWritenSize = 0;
	WriteFile(hFile, chBuffer, 4, &dwWritenSize, 0);
	CloseHandle(hFile);
}
BOOL MyDeleteFile(TCHAR* lpszPath)
{
	SHFILEOPSTRUCT FileOp = { 0 };
	FileOp.pFrom = lpszPath;
	FileOp.pTo = 0;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}










