#pragma once
#include "CPU.h"
#include <iostream>
#include <string.h>

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
				switch(input[4])
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

string* ParseString(string input)
{
	string* result = new string[3];
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

uint16_t* GetInstructionFromString(string input)
{
	uint16_t result[3];
	string* operands = new string[3];
	operands = ParseString(input);
	result[0] = DeEncryptIpCommandFromString(operands[0]);
	result[1] = DeEncryptOperandFromString(operands[1]);
	result[2] = DeEncryptOperandFromString(operands[2]);
	return result;
}