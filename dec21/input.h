#pragma once
#include <array>

#define regsize_input 6

int run_input(std::array<int, regsize_input>& reg)
{
	int ticks = 0;
	while (reg[4] < 31)
	{
		switch (reg[4])
		{
		case 0:
			ticks++;
			reg[1] = 123;
			reg[4]++;
		case 1:
			ticks++;
			reg[1] = reg[1] & 456;
			reg[4]++;
		case 2:
			ticks++;
			reg[1] = (reg[1] == 72);
			reg[4]++;
		case 3:
			ticks++;
			reg[4] = reg[1] + reg[4];
			break;
		case 4:
			ticks++;
			reg[4] = 0;
			break;
		case 5:
			ticks++;
			reg[1] = 0;
			reg[4]++;
		case 6:
			ticks++;
			reg[5] = reg[1] | 65536;
			reg[4]++;
		case 7:
			ticks++;
			reg[1] = 8586263;
			reg[4]++;
		case 8:
			ticks++;
			reg[2] = reg[5] & 255;
			reg[4]++;
		case 9:
			ticks++;
			reg[1] = reg[1] + reg[2];
			reg[4]++;
		case 10:
			ticks++;
			reg[1] = reg[1] & 16777215;
			reg[4]++;
		case 11:
			ticks++;
			reg[1] = reg[1] * 65899;
			reg[4]++;
		case 12:
			ticks++;
			reg[1] = reg[1] & 16777215;
			reg[4]++;
		case 13:
			ticks++;
			reg[2] = (256 > reg[5]);
			reg[4]++;
		case 14:
			ticks++;
			reg[4] = reg[2] + reg[4];
			break;
		case 15:
			ticks++;
			reg[4] = reg[4] + 1;
			break;
		case 16:
			ticks++;
			reg[4] = 27;
			break;
		case 17:
			ticks++;
			reg[2] = 0;
			reg[4]++;
		case 18:
			ticks++;
			reg[3] = reg[2] + 1;
			reg[4]++;
		case 19:
			ticks++;
			reg[3] = reg[3] * 256;
			reg[4]++;
		case 20:
			ticks++;
			reg[3] = (reg[3] > reg[5]);
			reg[4]++;
		case 21:
			ticks++;
			reg[4] = reg[3] + reg[4];
			break;
		case 22:
			ticks++;
			reg[4] = reg[4] + 1;
			break;
		case 23:
			ticks++;
			reg[4] = 25;
			break;
		case 24:
			ticks++;
			reg[2] = reg[2] + 1;
			reg[4]++;
		case 25:
			ticks++;
			reg[4] = 17;
			break;
		case 26:
			ticks++;
			reg[5] = reg[2];
			reg[4]++;
		case 27:
			ticks++;
			reg[4] = 7;
			break;
		case 28:
			ticks++;
			reg[2] = (reg[1] == reg[0]);
			reg[4]++;
		case 29:
			ticks++;
			reg[4] = reg[2] + reg[4];
			break;
		case 30:
			ticks++;
			reg[4] = 5;
			break;
		}
		reg[4]++;
	}
	return ticks;
}
