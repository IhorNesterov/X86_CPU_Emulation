#pragma once
#include <stdint.h>;
#include <iostream>;
#include "Memory.h"
#include "Register.h"
class CPU
{
private:
	Mem* memory;
	FlashMem* flash;
	StatusRegister sr;
	RegisterX16 op1;
	RegisterX16 op2;
	RegisterX16 ip;
	RegisterX16 sp;
	RegisterX16 pc;
	RegisterX16 ax;
	RegisterX16 bx;
	RegisterX16 cx;
	RegisterX16 dx;

public:
	CPU(Mem* memptr, FlashMem* fmemptr)
	{
		memory = memptr;
		flash = fmemptr;
		sr.SetData(0);
		op1.SetData(0);
		op2.SetData(0);
		ip.SetData(0);
		sp.SetData(0);
		pc.SetData(0);
		ax.SetData(0);
		bx.SetData(0);
		cx.SetData(0);
		dx.SetData(0);
	}

	void SetRegisterValueByNumber(uint16_t pos, uint16_t value)
	{
		switch (pos)
		{
		case 0x0001:
			ip.SetData(value);
			break;
		case 0x0002:
			op1.SetData(value);
			break;
		case 0x0003:
			op2.SetData(value);
			break;
		case 0x0004:
			sp.SetData(value);
			break;
		case 0x0005:
			pc.SetData(value);
			break;
		case 0x0006:
			ax.SetData(value);
			break;
		case 0x0007:
			bx.SetData(value);
			break;
		case 0x0008:
			cx.SetData(value);
			break;
		case 0x0009:
			dx.SetData(value);
			break;
		}
	}

	uint16_t GetRegisterValueByNumber(uint16_t pos)
	{
		switch (pos)
		{
		case 0x0001:
			return ip.GetData();
			break;
		case 0x0002:
			return op1.GetData();
			break;
		case 0x0003:
			return op2.GetData();
			break;
		case 0x0004:
			return sp.GetData();
			break;
		case 0x0005:
			return pc.GetData();
			break;
		case 0x0006:
			return ax.GetData();
			break;
		case 0x0007:
			return bx.GetData();
			break;
		case 0x0008:
			return cx.GetData();
			break;
		case 0x0009:
			return dx.GetData();
			break;
		}
	}

	void IncreaseRegister(RegisterX16* reg)
	{
		reg->SetData(reg->GetData() + 1);
	}

	void InstructionLoad(void)
	{
		ip.SetData(flash->GetInstruction(pc.GetData()));
		//Print();
		IncreaseRegister(&pc);
		op1.SetData(flash->GetInstruction(pc.GetData()));
		//Print();
		IncreaseRegister(&pc);
		op2.SetData(flash->GetInstruction(pc.GetData()));
		//Print();
		IncreaseRegister(&pc);
	}

	void DeEncryptInstruction(void)
	{
		switch (ip.GetData())
		{
		case 0x0000:
			delete memory;
			delete flash;
			break;

			//movrv Reg data  
		case 0x0001:
			SetRegisterValueByNumber(op1.GetData(), op2.GetData());
			break;

			//movrr Reg Reg
		case 0x0002:
			SetRegisterValueByNumber(op1.GetData(), GetRegisterValueByNumber(op2.GetData()));
			break;

			//movmr mem Reg
		case 0x0003:
			memory->SetShortBlock(op1.GetData(), GetRegisterValueByNumber(op2.GetData()));
			break;

			//movrm Reg mem
		case 0x0004:
			SetRegisterValueByNumber(op1.GetData(), memory->GetShortBlock(op2.GetData()));
			break;

			//movmv mem val
		case 0x0005:
			memory->SetShortBlock(op1.GetData(), op2.GetData());
			break;

			//add Reg Reg
		case 0x0010:
			SetRegisterValueByNumber(op1.GetData(), (GetRegisterValueByNumber(op1.GetData()) + GetRegisterValueByNumber(op2.GetData())));
			break;

			//add Reg val
		case 0x0011:
			SetRegisterValueByNumber(op1.GetData(), (GetRegisterValueByNumber(op1.GetData()) + op2.GetData()));
			break;

			//sub Reg Reg
		case 0x0020:
			SetRegisterValueByNumber(op1.GetData(), GetRegisterValueByNumber(op1.GetData()) - GetRegisterValueByNumber(op2.GetData()));
			break;

			//push val
		case 0x0030:
			SetRegisterValueByNumber(ip.GetData(), GetRegisterValueByNumber(op1.GetData()));
			break;


			//jmp val 
		case 0x0040:
			SetRegisterValueByNumber(1, op1.GetData());
			break;
			// je direction
		case 0x0041:
			if (sr.GetBit(2))
			{
				pc.SetData(op2.GetData());
			}
			else
			{
				pc.SetData(op1.GetData());
			}

			// jle direction
		case 0x0042:
			if (sr.GetBit(2) || sr.GetBit(3))
			{
				pc.SetData(op2.GetData());
			}
			else
			{
				pc.SetData(op1.GetData());
			}

			//cmprv reg value 
		case 0x0050:
			sr.ResetRegister();

			if (GetRegisterValueByNumber(op1.GetData()) > op2.GetData())
			{
				sr.SetBit(1, true);
			}
			else if (GetRegisterValueByNumber(op1.GetData()) == op2.GetData())
			{
				sr.SetBit(2, true);
			}
			else if (GetRegisterValueByNumber(op1.GetData()) < op2.GetData())
			{
				sr.SetBit(3, true);
			}
			break;

			//cmprr reg reg
		case 0x0051:
			sr.ResetRegister();

			if (GetRegisterValueByNumber(op1.GetData()) > GetRegisterValueByNumber(op2.GetData()))
			{
				sr.SetBit(1, true);
			}
			else if (GetRegisterValueByNumber(op1.GetData()) == GetRegisterValueByNumber(op2.GetData()))
			{
				sr.SetBit(2, true);
			}
			else if (GetRegisterValueByNumber(op1.GetData()) < GetRegisterValueByNumber(op2.GetData()))
			{
				sr.SetBit(3, true);
			}
			break;



		}
	}

	void Print(void)
	{
		cout << "**************************************" << endl;
		cout << "IP: " << ip.GetData() << endl;
		cout << "OP1: " << op1.GetData() << endl;
		cout << "OP2: " << op2.GetData() << endl;
		cout << "SR: " << sr << endl;
		cout << "SP: " << sp.GetData() << endl;
		cout << "PC: " << pc.GetData() << endl;
		cout << "AX: " << ax.GetData() << endl;
		cout << "BX: " << bx.GetData() << endl;
		cout << "CX: " << cx.GetData() << endl;
		cout << "DX: " << dx.GetData() << endl;
		cout << "**************************************" << endl << endl;
	}

	void Work(void)
	{
		InstructionLoad();
		DeEncryptInstruction();
		Print();
	}
};