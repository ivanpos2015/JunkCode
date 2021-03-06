#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <SpecialAPI.h>
#include <VersionHelpers.h>
#include <Winternl.h>
using namespace std;
WCHAR*   WINAPI  GetCommandLineByName(TCHAR* Name);
typedef NTSTATUS(WINAPI *ZWQUERYINFORMATIONPROCESS)
 (
	HANDLE           ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID            ProcessInformation,
	ULONG            ProcessInformationLength,
	PULONG           ReturnLength
);

int main()
{
	locale loc("chs");
	WCHAR* Name = L"Idr.exe";
	Name = GetCommandLineByName(Name);

	printf("%ls \n", Name);
	cout << endl;
	system("pause");

}
WCHAR*  WINAPI  GetCommandLineByName(TCHAR* Name)
{
	HANDLE hProcess = GetProcessHandleByName(Name);
	if (hProcess == 0)   return 0;
	PROCESSINFOCLASS ProcessInfo1 = ProcessBasicInformation;
	PROCESS_BASIC_INFORMATION ProcessInfo2;
	ZeroMemory(&ProcessInfo2, sizeof(ProcessInfo2));
	ULONG ProcessInformationLength = sizeof(ProcessInfo2);
	PULONG   ReturnLength=&ProcessInformationLength;
	HMODULE hFile = LoadLibrary(L"C:\\WINDOWS\\System32\\ntdll.dll");
	LPCSTR p = "ZwQueryInformationProcess";
	ZWQUERYINFORMATIONPROCESS ZwQueryInformationProcess=(ZWQUERYINFORMATIONPROCESS)GetProcAddress(hFile,p);
	ZwQueryInformationProcess(hProcess, ProcessInfo1,&ProcessInfo2, ProcessInformationLength, ReturnLength);
	//DWORD PebAddress[2] = { 0,0 };

	BYTE PebBase[sizeof(PEB)] = { 0 };
	BYTE ProcessPara[sizeof(PRTL_USER_PROCESS_PARAMETERS)] = { 0 };
	BYTE Buffer[200] = { 0 };

	ReadProcessMemory(hProcess,ProcessInfo2.PebBaseAddress,PebBase, sizeof(PEB),0);
	PPEB Peb = (PPEB)PebBase;
	ReadProcessMemory(hProcess, Peb->ProcessParameters, ProcessPara, sizeof(RTL_USER_PROCESS_PARAMETERS),0);
	PRTL_USER_PROCESS_PARAMETERS ProcessParam = (PRTL_USER_PROCESS_PARAMETERS)ProcessPara;
	
	UNICODE_STRING CommandLine1 = ProcessParam->CommandLine;
	WCHAR* p1 = CommandLine1.Buffer;
	ReadProcessMemory(hProcess, p1, Buffer, 200, 0);




    //UNICODE_STRING CommandLine1 = ProcessInfo2.PebBaseAddress->ProcessParameters->CommandLine;
	//WCHAR* p1 = CommandLine1.Buffer;
	return (WCHAR*)Buffer;
}




