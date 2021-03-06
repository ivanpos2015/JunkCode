#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;
#define PAUSE system("pause")
#define ENDL  cout<<endl
typedef struct NODE
{
	int data;
	NODE* Next;
};
typedef NODE* PNODE;
PNODE CreateEmptyList();
PNODE CreateNewNode();
int main()
{
	cout << "Press Any Key To Create An Empty List......" << endl;
	PAUSE;
	ENDL;
	PNODE p = CreateEmptyList();
	p->data = 0x12345678;
	printf("New List Head Address Is: 0x%08X \n", (DWORD)p);
	ENDL;


	cout << "Press Any Key To Create A New Node And Link It After Head....." << endl;
	PNODE p1 = CreateNewNode();
	p1->data = 0xABCDEF;
	p->Next = p1;
	printf("The New Node Address Is: 0x%08X \n", (DWORD)p1);
	printf("List Head Now Is : 0x%08X \n", p->data);
	ENDL;



	cout << "Press Any Key To Create A New Node And Link It After Head....." << endl;
	PNODE p2 = CreateNewNode();
	p2->data = 0xABCD;
	p1->Next = p2;
	printf("The New Node Address Is: 0x%08X \n", (DWORD)p2);
	
	ENDL;


	printf("List Head Now Is : 0x%08X \n", p);
	printf("List Head Data Now Is : 0x%08X \n", p->data);
	printf("Next 1 Node Is : 0x%08X \n", p->Next);
	printf("Next 2 Node Is : 0x%08X \n", p1->Next);


	cout << "Press Any Key To Insert A New Node Between Node1 And Node 2....." << endl;
	PAUSE;

	PNODE p3 = CreateNewNode();
	p1->Next = p3;
	p3->Next = p2;

	printf("Next 1 Node Is : 0x%08X \n", p->Next);
	printf("Next 2 Node Is : 0x%08X \n", p1->Next);
	printf("Next 3 Node Is : 0x%08X \n", p3->Next);



	PAUSE;

    return 0;
}
PNODE CreateEmptyList()
{
	HANDLE hHeap=HeapCreate(HEAP_GENERATE_EXCEPTIONS, sizeof(NODE), sizeof(NODE));
	PNODE p = (PNODE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(NODE));

	return p;
}
PNODE CreateNewNode()
{

	HANDLE hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, sizeof(NODE), sizeof(NODE));
	PNODE p = (PNODE)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(NODE));

	return p;


}

