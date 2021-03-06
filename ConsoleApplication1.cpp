// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdlib.h>
#include <math.h>

const double SPLIT = 0.01;
const int Count = 200;
const double PI = 3.1415926;
const int INTERVAL = 300;

int main()
{   
	DWORD busyspan[Count];
	DWORD idlespan[Count];
	int half = INTERVAL / 2;
	double radian = 0.0;
	for (int i = 0; i < Count; i++)
	{
		busyspan[i] = (DWORD)(half + (sin(PI*radian)*half));
		idlespan[i] = INTERVAL - busyspan[i];
		radian += SPLIT;

	}
	DWORD starttime = 0;
	int j = 0;
	while (true)
	{
		j = j%Count;
		starttime = GetTickCount();
		while ((GetTickCount() - starttime) <= busyspan[j])
			;
		Sleep(idlespan[j]);
		j++;
	}


    return 0;
}

