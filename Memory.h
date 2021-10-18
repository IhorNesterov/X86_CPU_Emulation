#pragma once
#include <stdint.h>
#include <iostream>
#include <bitset>
#include <string.h>
#include <fstream>

using namespace std;
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

	void ManualSetInstructrion(uint16_t pos, uint16_t _ip, uint16_t _op1, uint16_t _op2)
	{
		data[pos] = _ip;
		data[pos + 1] = _op1;
		data[pos + 2] = _op2;
	}

	void SetInstruction(uint16_t* dat)
	{
		data[dat[0]] = dat[1];
		data[dat[0] + 1] = dat[2];
		data[dat[0] + 2] = dat[3];
	}

	string* ParseString(string input)
	{
		string* result = new string[4];
		int pos = 0;
		for (int i = 0; i < size(input); ++i)
		{
			if (input[i] == ' ')
			{
				++pos;
			}
			else
			{
				result[pos] += input[i];
			}
		}
		return result;
	}

	uint16_t DeEncryptIpCommandFromString(string input)
	{
		switch (input[0])
		{
		case 'm':
			if (input[1] == 'o' && input[2] == 'v')
			{
				switch (input[3])
				{
				case 'r':
					switch (input[4])
					{
					case 'r':
						return 0x0002; //movrr
						break;
					case 'v':
						return 0x0001; //movrv
						break;
					case 'm':
						return 0x0004; //movrm
						break;
					}
					break;
				case 'm':
					switch (input[4])
					{
					case 'r':
						return 0x0003; //movrr
						break;
					case 'v':
						return 0x0005; //movrv
						break;
					}
					break;
				}
			}
			break;

		case 'a':
			if (input[1] == 'd' && input[2] == 'd')
			{
				switch (input[3])
				{
				case 'r':
					switch (input[4])
					{
					case 'r':
						return 0x0010; //addrr
						break;
					case 'v':
						return 0x0011; //addrv
						break;
					}
					break;
				}
			}
		}
	}

	bool isNumber(char input)
	{
		char temp = '0';
		for (int i = 0; i < 10; ++i)
		{
			if (input == temp)
			{
				return true;
			}
			temp++;
		}
		return false;
	}

	uint16_t DeEncryptOperandFromString(string input)
	{
		if (isNumber(input[0]))
		{
			return stoi(input);
		}
		else
		{
			switch (input[0])
			{
			case 'a':
				switch (input[1])
				{
				case 'x':
					return 0x0006;
					break;
				}
				break;

			case 'b':
				switch (input[1])
				{
				case 'x':
					return 0x0007;
					break;
				}
				break;

			case 'c':
				switch (input[1])
				{
				case 'x':
					return 0x0008;
					break;
				}
				break;

			case 'd':
				switch (input[1])
				{
				case 'x':
					return 0x0009;
					break;
				}
				break;


			}

		}
	}

	uint16_t* GetInstructionFromString(string input)
	{
		uint16_t result[4];
		string* operands = new string[4];
		operands = ParseString(input);
		result[0] = stoi(operands[0]);
		result[1] = DeEncryptIpCommandFromString(operands[1]);
		result[2] = DeEncryptOperandFromString(operands[2]);
		result[3] = DeEncryptOperandFromString(operands[3]);
		return result;
	}

	void LoadCodeFile(string path)
	{
		ifstream file(path);
		string temp;
		int InstructionsCount = 0;
		getline(file, temp);
		InstructionsCount = stoi(temp);
		for (int i = 0; i < InstructionsCount; i++)
		{
			getline(file, temp);
			SetInstruction(this->GetInstructionFromString(temp));
		}
	}
};