#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <Winternl.h>
using namespace std;
BYTE Sign1[16] = { 0x8B,0x44,0x24,0x0C,0x8B,0x4C,0x24,0x8,0x8B,0x54,0x24,0x04,0x50,0x51,0x52,0xB9 };
BYTE Sign2[38] =
{ 
	0x83,0xEC,0x0C,0x33,0xC0 ,0x56 ,0x8B ,0x74 ,0x24 ,0x1C,0x57,0x8B ,0x7C,0x24 ,0x18 ,0xC7 ,0x07,
    0x00,0x00 ,0x00 ,0x00 ,0x8B ,0x4E ,0x14 ,0x85 ,0xC9,0x74 ,0x13 ,0x50,0x8B,0x46 ,0x0C,0x50 ,0x68,
	0xD6,0x07,0x00 ,0x00
};
BYTE InjectCode[129] = 
{ 
 0xC8,0x00,0x00 ,0x00 ,0x68 ,0x02 ,0x00 ,0x00 ,0x80 ,0x68 ,0x00 ,0x00 ,0x00 ,0x00 ,0x68 ,0x01
,0x00 ,0x00 ,0x00 ,0x68 ,0x00 ,0x00 ,0x00 ,0x00 ,0x68 ,0x00 ,0x00 ,0x00 ,0x00 ,0x68 ,0x00,0x00
,0x00 ,0x00 ,0x68 ,0x01 ,0x00 ,0x01 ,0x00 ,0x68 ,0x02 ,0x00 ,0x01 ,0x06 ,0x68 ,0x00 ,0x00 ,0x00
,0x00,0x68 ,0x03 ,0x00 ,0x00 ,0x00 ,0xBB ,0x00 ,0x00,0x00,0x00 ,0xE8,0x08 ,0x00 ,0x00 ,0x00
,0x81 ,0xC4 ,0x28 ,0x00 ,0x00 ,0x00 ,0xC9 ,0xC3,0x8D ,0x44 ,0x24 ,0x08 ,0x81 ,0xEC ,0x0C,0x00
,0x00 ,0x00 ,0x50 ,0xFF ,0x74 ,0x24 ,0x14 ,0x31 ,0xC0 ,0x89 ,0x44 ,0x24 ,0x08 ,0x89 ,0x44 ,0x24
,0x0C ,0x89 ,0x44 ,0x24 ,0x10 ,0x8D ,0x54 ,0x24 ,0x08 ,0x52 ,0xFF ,0xD3 ,0x8B ,0x44 ,0x24 ,0x0C
,0x8B ,0x54 ,0x24 ,0x10 ,0x8B ,0x4C ,0x24 ,0x14 ,0x81 ,0xC4 ,0x18 ,0x00 ,0x00 ,0x00 ,0xC2 ,0x04
,0x00
};//窗口ID是45,46,47,48号,载入函数是55,56,57,58
DWORD SigCompare(PBYTE TextSection, DWORD Size);
VOID TextToTemp(PBYTE Temp, PBYTE Text);
DWORD SigCompare2(PBYTE TextSection, DWORD Size);
VOID TextToTemp2(PBYTE Temp, PBYTE Text);
BOOL ElevateDebugPrivileges();
BOOL WindowIdCheck(DWORD WindowId);
int main()
{
	ElevateDebugPrivileges();
	begin:
	DWORD ProcessId;
	DWORD WindowsId;
	DWORD NumberOfBytesRead;
	DWORD Result;
	DWORD WindowInfoBase;
	DWORD WindowInfo[8] = { 0 };
	DWORD LoadFunc;
	HANDLE TargetProcess;


	cout << "Input The Target Process ID: " << endl;
	cin >> ProcessId;
	TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
	if (TargetProcess == 0)
	{
		cout << "Invalid Handle." << endl;
		goto begin;
	}


	MEMORY_BASIC_INFORMATION MemInfo = { 0 };
	VirtualQueryEx(TargetProcess, (PVOID)0x00401000, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION));

	PDWORD TextSection=(PDWORD)VirtualAlloc(0, MemInfo.RegionSize, MEM_COMMIT, PAGE_READWRITE);
	PDWORD Temp = TextSection;
	TextSection[0] = 1;
	TextSection[1] = MemInfo.RegionSize;
	ReadProcessMemory(TargetProcess, (PVOID)0x00401000, TextSection + 2, MemInfo.RegionSize, &NumberOfBytesRead);

	Result=SigCompare((PBYTE)TextSection, MemInfo.RegionSize);
	LoadFunc = SigCompare2((PBYTE)Temp, MemInfo.RegionSize)+0x00401000;

	if (Result == 0)
	{
		cout << "Signature Not Found ." << endl;
		goto begin;
	}

	ReadProcessMemory(TargetProcess, (PVOID)(0x00401000 + Result + 16), &WindowInfoBase, 4, &NumberOfBytesRead);

	ReadProcessMemory(TargetProcess, (PVOID)(WindowInfoBase +0x104), &WindowInfo, 32, &NumberOfBytesRead);

	PDWORD pWindowsSig= (PDWORD)VirtualAlloc(0, WindowInfo[6] / 8*4, MEM_COMMIT, PAGE_READWRITE);

	ReadProcessMemory(TargetProcess, (PVOID)(WindowInfo[4]), pWindowsSig, WindowInfo[6] / 8 * 4, &NumberOfBytesRead);
	cout << endl;


	printf("Windows Info: 0x%08X \n", WindowInfoBase);
	printf("Windows Number: 0x%08X \n", WindowInfo[6]/8);
	printf("E-Language Signature Offset: 0x%08X \n", Result);
	cout << endl;
	printf("Window Load Function Entry: 0x%08X \n", LoadFunc);



	int i = 0;
	while (*pWindowsSig != 0)
	{
		printf("Windows ID: 0x%08X \n", *pWindowsSig);
		pWindowsSig++;
		i++;
    }
	printf("There Are %d Windows In This E-Language Program. \n",i);

Input:

	cout << "Input A Windows ID To Inject: " << endl;
	scanf_s("%X", &WindowsId);

	if (WindowIdCheck(WindowsId) == 0)
	{
		cout << "Invalid WindowId,Please Input Again" << endl;
		goto Input;
	}

	PBYTE pInject=(PBYTE)VirtualAllocEx(TargetProcess, 0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	PBYTE pLoadFunc = (PBYTE)(&LoadFunc);
	PBYTE pWindowsId = (PBYTE)(&WindowsId);

	for (int i = 0; i <= 3; i++)
	{
		InjectCode[55 + i] = *pLoadFunc;
		pLoadFunc++;
	}
	for (int i = 0; i <= 3; i++)
	{
		InjectCode[45 + i] = *pWindowsId;
		pWindowsId++;
	}

	if (WriteProcessMemory(TargetProcess, pInject, InjectCode,129, NULL) == 0)
	{
		cout << "Code Inject Fail.";
		printf("Error Code: 0x%X \n", GetLastError());
		goto begin;
	}

	CreateRemoteThread(TargetProcess, 0, 0, (LPTHREAD_START_ROUTINE)pInject, 0, 0, 0);

	cout << "注入成功." << endl;
	CHAR YN;

	cout << "是否继续输入窗口ID进行注入测试?(Y/N)" << endl;

	cin >> YN;
	if (YN == 'y' || YN == 'Y') goto Input;
	if (YN == 'N' || YN == 'n')
	{
		system("pause");
		return 0;
	}
	cout << endl;
	//goto begin;
	system("pause");
    return 0;
}
DWORD SigCompare(PBYTE TextSection,DWORD Size)
{
	int x = 0;
	int Count = 0;
	int i = 0;
	BYTE Temp[16] = { 0 };
	PBYTE OrgTextSection = TextSection;
	while (i <= Size)
	{
		TextToTemp(Temp,TextSection);

		for (x = 0; x <= 15; x++)
		{
			if (Sign1[x] == Temp[x])
			{
				Count++;
			}
		}
		TextSection++;
		i++;
		if (Count == 16) break;
		Count = 0;
    }


	if (Count == 16) return (DWORD)(TextSection-OrgTextSection-9);

	else return 0;
}
VOID TextToTemp(PBYTE Temp,PBYTE Text)
{
	PBYTE Text1 = Text;
	for (int i = 0; i <= 15; i++)
	{
		*Temp = *Text1;
		Temp++;
		Text1++;
	}
}

DWORD SigCompare2(PBYTE TextSection, DWORD Size)

{
	int x = 0;
	int Count = 0;
	int i = 0;
	BYTE Temp[38] = { 0 };
	PBYTE OrgTextSection = TextSection;
	while (i <= Size)
	{
		TextToTemp2(Temp, TextSection);

		for (x = 0; x <= 37; x++)
		{
			if (Sign2[x] == Temp[x])
			{
				Count++;
			}
		}
		TextSection++;
		i++;
		if (Count == 38) break;
		Count = 0;
	}


	if (Count == 38) return (DWORD)(TextSection - OrgTextSection - 9);

	else return 0;
}
VOID TextToTemp2(PBYTE Temp, PBYTE Text)
{
	PBYTE Text1 = Text;
	for (int i = 0; i <= 37; i++)
	{
		*Temp = *Text1;
		Temp++;
		Text1++;
	}
}
BOOL WindowIdCheck(DWORD WindowId)
{
	WindowId &= 0xFF000000;
	WindowId /= 0x1000000;
	if (WindowId==0x52) return 1;
	else return 0;
}
BOOL ElevateDebugPrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))//取得进程令牌句柄.
		return FALSE;//失败返回0.
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);//获取对其他进程进行调试的特权.
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))//设定打开该特权
	{
		return FALSE;
	}
	return TRUE;
}