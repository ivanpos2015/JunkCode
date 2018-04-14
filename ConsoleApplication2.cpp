#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
using namespace std;
int main()
{   
	PROCESSENTRY32 PE32;
	PE32.dwSize = sizeof(PE32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	 if (hProcessSnap==INVALID_HANDLE_VALUE)
	 {
		 cout << "Call Fail!" << endl;
		 return  -1;
	 }
	 BOOL b = Process32First(hProcessSnap, &PE32);
	 DWORD ProcessID;
	 while (b)
	 {    
		 b = Process32Next(hProcessSnap, &PE32);
		 if ((wcscmp(PE32.szExeFile,L"QQPCTray.exe")==0))
		 {
			 ProcessID = PE32.th32ProcessID;
			 if (TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID), 0) == 1)
		     cout << "Terminate Success" << endl;
			 else  GetLastError();
			 cout << GetLastError()<< endl;
		 } 
		 printf("Process Name is:%ls \n", PE32.szExeFile);
		 cout << "Process PID is:" << PE32.th32ProcessID << endl;
		 cout << endl;
	  }
	    // cout << "There is " << i << " Process in This Table" << endl;
		 CloseHandle(hProcessSnap);
	     system("pause");
		 return 0;
}
