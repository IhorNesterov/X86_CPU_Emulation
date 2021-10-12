#include <iostream>
#include <bitset>
using namespace std;
class RegisterX8
{
private:
	uint8_t data;

public:
	uint8_t GetData(void)
    {
		return data;
	}

	void SetData(uint8_t dat)
	{
		data = dat;
	}
};

typedef union DoubleReg_t
{
	RegisterX8 reg[2];
	uint16_t data;
}DoubleReg;

typedef union Short_T
{
public:
	uint8_t bytes[2];
	uint16_t data;
}Short;

class RegisterX16
{
private:
	DoubleReg data;

public:
	uint16_t GetData(void)
	{
		return data.data;
	}

	void SetData(uint16_t val)
	{
		data.data = val;
	}

	friend std::ostream& operator << (std::ostream& out, const RegisterX16& in)
	{
		out << bitset<16>(in.data.data);
		return out;
	}
};

class StatusRegister : public RegisterX16
{
public:
	void SetBit(uint16_t pos,bool state)
	{
		uint16_t buff = GetData();
		if (state)
		{
			SetData(buff |= (1 << pos));
		}
		else
		{
			SetData(buff &= ~(1 << pos));
		}
	}

	bool GetBit(uint16_t pos)
	{
		uint16_t buff = GetData();
		if (buff & (1 << pos))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ResetRegister(void)
	{
		for (int i = 0; i < 16; i++)
		{
			SetBit(i, false);
		}
	}

};

class Mem
{
private:
	uint16_t* data;
	uint32_t size;

public:
	Mem(uint32_t _size)
	{
		data = new uint16_t[_size];
		size = _size;
	}

	uint8_t GetByteBlock(uint32_t pos)
	{
		return data[pos];
	}

	uint16_t GetShortBlock(uint16_t pos)
	{
		return data[pos];
	}

	void SetByteBlock(uint8_t dat, uint32_t block)
	{
		data[block] = dat;
	}

	void SetShortBlock(uint16_t block, uint16_t dat)
	{
		data[block] = dat;
	}
};

class FlashMem
{
private:
	uint16_t* data;

public:
	FlashMem(uint32_t size)
	{
		data = new uint16_t[size];
	}

	uint16_t GetInstruction(uint32_t pos)
	{
		return data[pos];
	}

	void SetInstructrion(uint16_t pos,uint16_t _ip, uint16_t _op1, uint16_t _op2)
	{
		data[pos] = _ip;
		data[pos + 1] = _op1;
		data[pos + 2] = _op2;
	}
};

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

			//add Reg Reg
		case 0x0003:
			SetRegisterValueByNumber(op1.GetData(),(GetRegisterValueByNumber(op1.GetData()) + GetRegisterValueByNumber(op2.GetData())));
			break;

			//add Reg val
		case 0x000E:
			SetRegisterValueByNumber(op1.GetData(), (GetRegisterValueByNumber(op1.GetData()) + op2.GetData()));
			break;

			//sub Reg Reg
		case 0x0004:
			SetRegisterValueByNumber(op1.GetData(), GetRegisterValueByNumber(op1.GetData()) - GetRegisterValueByNumber(op2.GetData()));
			break;

			//push val
		case 0x0005:
			SetRegisterValueByNumber(ip.GetData(), GetRegisterValueByNumber(op1.GetData()));
			break;

			//movmr mem Reg
		case 0x0006:
			memory->SetShortBlock(op1.GetData(),GetRegisterValueByNumber( op2.GetData()));
			break;

			//movrm Reg mem
		case 0x0007:
			SetRegisterValueByNumber(op1.GetData(), memory->GetShortBlock(op2.GetData()));
			break;

			//movmv mem val
		case 0x0008:
			memory->SetShortBlock(op1.GetData(), op2.GetData());
			break;

			//jmp val 
		case 0x0009:
			SetRegisterValueByNumber(1, op1.GetData());
			break;

			//cmprv reg value 
		case 0x000A:
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
		case 0x000B:
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

           // je direction
		case 0x000C:
			if (sr.GetBit(2))
			{
				pc.SetData(op2.GetData());
			}
			else
			{
				pc.SetData(op1.GetData());
			}

			// jle direction
		case 0x000D:
			if (sr.GetBit(2) || sr.GetBit(3))
			{
				pc.SetData(op2.GetData());
			}
			else
			{
				pc.SetData(op1.GetData());
			}

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

int main(void)
{
	int a = 0;
	Mem* RAMmemory = new Mem(1024);
	FlashMem* Flashmemory = new FlashMem(1024);

	CPU* cpu = new CPU(RAMmemory, Flashmemory);
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
	Flashmemory->SetInstructrion(42, 0x000D,45, 33); // jle 33,45;
	Flashmemory->SetInstructrion(45, 1, 9, 3);       // movrv dx,3;

	for (int i = 0; i < 40; i++)
	{
		cpu->Work();
	}
	cin >> a;

}