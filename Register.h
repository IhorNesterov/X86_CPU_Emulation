#pragma once
#include <stdint.h>
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
	void SetBit(uint16_t pos, bool state)
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