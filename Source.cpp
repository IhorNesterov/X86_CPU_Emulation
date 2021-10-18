#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include "CPU.h"
#include "Compiler.h"

using namespace std;

int main(void)
{
	int a = 0;
	string input;
	Mem* RAMmemory = new Mem(1024);
	FlashMem* Flashmemory = new FlashMem(1024);
	CPU* cpu = new CPU(RAMmemory, Flashmemory);
	Flashmemory->LoadCodeFile("C:\\1\\Code.txt");

	for (int i = 0; i < 11; i++)
	{
		cpu->Work();
	}
	cin >> a;

}

/*test instructions code
	                                                 // word ptr[a] = 1
	                                                 // word ptr[b] = 2
	Flashmemory->SetInstructrion(0, 8, 1, 25);       // movmv a,25; 
	Flashmemory->SetInstructrion(3, 8, 2, 15);       // movmv 2,15;
	Flashmemory->SetInstructrion(6, 7, 6, 1);        // movrm ax,a;
	Flashmemory->SetInstructrion(9, 7, 7, 2);        // movrm bx,2;
	Flashmemory->SetInstructrion(12, 3, 6, 7);       // addrr ax,bx;
	Flashmemory->SetInstructrion(15, 6, 3, 6);       // movmr 3,ax;
	Flashmemory->SetInstructrion(18, 7, 8, 3);       // movrm cx,3;
	Flashmemory->SetInstructrion(21, 1, 6, 3);       // movrv ax,3;
	Flashmemory->SetInstructrion(24, 1, 7, 3);       // movrv bx,3;
	Flashmemory->SetInstructrion(27, 1, 8, 3);       // movrv cx,3;
	Flashmemory->SetInstructrion(30, 1, 9, 0);       // movrv dx,0;
	Flashmemory->SetInstructrion(33, 3, 6, 7);       // addrr ax,bx;
	Flashmemory->SetInstructrion(36, 0x000E, 9, 1);  // addrv dx,1;
	Flashmemory->SetInstructrion(39, 0x000B, 9, 8);  // cmprr dx,cx;
	Flashmemory->SetInstructrion(42, 0x000D,45, 33); // jle 45,33;
	Flashmemory->SetInstructrion(45, 1, 9, 3);       // movrv dx,3;
*/