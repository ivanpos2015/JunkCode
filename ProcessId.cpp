#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
using namespace std;
int main()
{
	DWORD ID;
	HANDLE hSnapShot;
	LPCSTR Format = "%d \n";
	PROCESSENTRY32 ProcessEntry = { 0 };
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	LPCWSTR ProcessName = L"QQ.exe";
	DWORD Wcscmp = (DWORD)wcscmp;
	__asm
	{
		PUSH 0
		PUSH TH32CS_SNAPPROCESS
		CALL CreateToolhelp32Snapshot
		MOV hSnapShot,EAX
		LEA EAX,ProcessEntry
		PUSH EAX
		PUSH hSnapShot
		CALL Process32First
		TEST EAX,EAX
		JE label0

		label1:
		LEA EAX, ProcessEntry
		LEA EAX,[EAX+0x24]
		PUSH EAX
		MOV EAX,ProcessName
		PUSH EAX
		MOV EAX,Wcscmp
		CALL EAX
		CMP EAX,0
		JE label2

		label3:
		LEA EAX, ProcessEntry
		PUSH EAX
		PUSH hSnapShot
		CALL Process32Next
		TEST EAX,EAX
		JE  label0
		JNZ label1


	 label2:
		LEA EAX, ProcessEntry
		MOV EAX,[EAX+8]
        PUSH EAX
		MOV EAX,Format
		PUSH EAX
		CALL printf
		JMP label3

	label0 :
		PUSH hSnapShot
			CALL CloseHandle

	}

	system("pause");
	
    return 0;
}

