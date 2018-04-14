#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <SpecialAPI.h>
#include <VersionHelpers.h>
#include <Winternl.h>
using namespace std;
WCHAR*   WINAPI  GetCommandLineByName(TCHAR* Name);
int main()
{   
	
	WCHAR* Name = L"QQ.exe";
	Name = GetCommandLineByName(Name);
	printf("%S", Name);
	system("pause");

}
BOOL DebuggerCheck()
{
	PROCESSINFOCLASS ProcessInfo1 = ProcessBasicInformation;
	PROCESS_BASIC_INFORMATION ProcessInfo2;
	ZeroMemory(&ProcessInfo2, sizeof(ProcessInfo2));
	ULONG ProcessInformationLength = sizeof(ProcessInfo2);
	PULONG   ReturnLength = 0;
	HMODULE hFile = LoadLibrary(L"C:\\WINDOWS\\System32\\ntdll.dll");
	LPCSTR p = "ZwQueryInformationProcess";
	GetProcAddress(hFile, p);
	__asm
	{
		    push ReturnLength;
		    push ProcessInformationLength;
		    lea ebx, ProcessInfo2
			push ebx
			push ProcessInfo1
			push - 1
			call eax
	}

	BOOL DebugFlag = ProcessInfo2.PebBaseAddress->BeingDebugged;
	if (DebugFlag == 1) return 1;
	if (DebugFlag == 0) return 0;

}

 WCHAR*   WINAPI  GetCommandLineByName(TCHAR* Name)
{
	HANDLE hProcess = GetProcessHandleByName(Name);
	if (hProcess == 0)return 0;
	PROCESSINFOCLASS ProcessInfo1 = ProcessBasicInformation;
	PROCESS_BASIC_INFORMATION ProcessInfo2;
	ZeroMemory(&ProcessInfo2, sizeof(ProcessInfo2));
	ULONG ProcessInformationLength = sizeof(ProcessInfo2);
	PULONG   ReturnLength = 0;
	HMODULE hFile = LoadLibrary(L"C:\\WINDOWS\\System32\\ntdll.dll");
	LPCSTR p = "ZwQueryInformationProcess";
	GetProcAddress(hFile, p);
	__asm
	{
		    push ReturnLength;
		    push ProcessInformationLength;
		    lea ebx, ProcessInfo2
			push ebx
			push ProcessInfo1
			push hProcess
			call eax
	}
	UNICODE_STRING CommandLine1 = ProcessInfo2.PebBaseAddress->ProcessParameters->CommandLine;
	WCHAR* p1 = CommandLine1.Buffer;
	return p1;
}


