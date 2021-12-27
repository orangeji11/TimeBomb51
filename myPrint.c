#include<reg52.h>
#include"myPrint.h"

typedef unsigned int u16;
typedef unsigned char u8;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
xdata unsigned char numTable[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char printStr[10] = "";

unsigned char dict(char x) {
	if (x >= '0' && x <= '9') {
		return numTable[x - '0'];
	} else if (x >= 'A' && x <= 'Z') {
		x += 'a' - 'A';
	}
	switch (x) {
		default : return 0; 	break;
		case '-': return 0x40; 	break;
		case '_': return 8; 	break;
		case 'a': return 119; 	break;
		case 'b': return 124; 	break;
		case 'c': return 0x39; 	break;
		case 'd': return 94; 	break;
		case 'g': return numTable[9]; break;
		case 'm': return 55; 	break;
		case 'n': return 84; 	break;
		case 'o': return 92; 	break;
		case 'r': return 80; 	break;
		case 's': return numTable[5]; break;
		case 't': return 120; 	break;
		case 'y': return 110; 	break;
	}
}

void delay_10us(u16 ten_us)
{
	while(ten_us--);	
}

void print(void) {
	char i;
	for (i=7; i>=0; --i) {
		switch(i)
		{
			case 0: LSC=1;LSB=1;LSA=1;break;
			case 1: LSC=1;LSB=1;LSA=0;break;
			case 2: LSC=1;LSB=0;LSA=1;break;
			case 3: LSC=1;LSB=0;LSA=0;break;
			case 4: LSC=0;LSB=1;LSA=1;break;
			case 5: LSC=0;LSB=1;LSA=0;break;
			case 6: LSC=0;LSB=0;LSA=1;break;
			case 7: LSC=0;LSB=0;LSA=0;break;
		}
		P0 = dict(printStr[i]);
		delay_10us(100);
		P0 = 0x00;
	}
}

