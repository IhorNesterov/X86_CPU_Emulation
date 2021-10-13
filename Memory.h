#pragma once
#include <stdint.h>

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

	void SetInstructrion(uint16_t pos, uint16_t _ip, uint16_t _op1, uint16_t _op2)
	{
		data[pos] = _ip;
		data[pos + 1] = _op1;
		data[pos + 2] = _op2;
	}
};