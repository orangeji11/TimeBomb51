#include<reg52.h>
#include<string.h>
#include<stdio.h>
#include"myPrint.h"
#include"myBuzzer.h"
typedef unsigned int u16;
typedef unsigned char u8;

unsigned long explodeTime = 45, defuseTime = 8, plantTime = 8;
unsigned long cnt1ms = 0;

sbit button = P3^1;
sbit buzzer = P2^5;

void Timer1Int_1ms(void) interrupt 3 {
	TH1 = 0xFC;
	TL1 = 0x18;
	TR1 = 1;
	++ cnt1ms;
}

void delayms(unsigned long time) {
	unsigned long t1 = cnt1ms + time;
	while (cnt1ms < t1);
}

/*
void playSong(void) {
	xdata char score[] = "115566550044332211005544332200554433220011556655004433221100";
	int len = strlen(score), i;
	TR1 = 0;
	for (i=0; i<len; ++i) {
		beep(score[i] - '0', 400);
	}
}
*/

void exploded() {
	int i;
	strcpy(printStr, "    boom");
	print();
	for (i=670; i>=20; i-=10) {
		beep(i, 54-0.08*i);
	}
	while (1) {
		if (cnt1ms % 2000 < 1000) {
			strcpy(printStr, "    boom");
		} else {
			strcpy(printStr, "        ");
		}
		print();
	}
}

void defused(void) {
	int i;
	strcpy(printStr, "congrats");
	print();
	for (i=3; i>=1; --i) {
		beep(-i, 100);
	}
	while (1) {
		if (cnt1ms % 2000 < 1000) {
			strcpy(printStr, "congrats");
		} else {
			strcpy(printStr, "        ");
		}
		print();
	}
}

void defusing(unsigned long bombEndTime) {
	unsigned long startTime = cnt1ms, endTime = cnt1ms + 1000 * defuseTime;
	memset(printStr, 0, sizeof printStr);
	
	while (cnt1ms < endTime) {
		printStr[(cnt1ms - startTime) / 125 / defuseTime] = '-';
		print();
		
		setFreq(-10);
		if ((cnt1ms - startTime) % 500 < 100) {
			TR0 = 1;
		} else {
			TR0 = 0;
		}
		
		if (button == 1) {
			delayms(50);
			if (button == 1) {
				TR0 = 0;
				return ;
			}
		}
		if (cnt1ms >= bombEndTime) {
			TR0 = 0;
			exploded();
		}
	}
	TR0 = 0;
	defused();
}

void planted(void) {
	unsigned long startTime = cnt1ms, endTime = cnt1ms + 1000 * explodeTime, preBombTime1 = explodeTime/3 , preBombTime2 = preBombTime1*2;
	char flg = 0;
	int i;
	for (i=1; i<=3; ++i) {
		beep(-i, 100);
	}
	while (cnt1ms < endTime) {
		sprintf(printStr, "%8ld", (endTime - cnt1ms)/1000+1);
		print();

		if(((endTime - cnt1ms)/1000+1) > preBombTime2) {
			setFreq(-10);
			if ((cnt1ms - startTime) % 1000 < 200) {
				buzzer = 1;
			} else {
				buzzer = 0;
			}
		} else if (((endTime - cnt1ms)/1000+1) > preBombTime1) {
			setFreq(-10);
			if ((cnt1ms - startTime) % 500 < 100) {
				TR0 = 1;
			} else {
				TR0 = 0;
			}
		} else {
			setFreq(-12);
			if ((cnt1ms - startTime) % 100 < 40) {
				TR0 = 1;
			} else {
				TR0 = 0;
			}
		}
			
		if (button == 0) {
			if (flg) {												
				delayms(50);
				buzzer = 0;
				defusing(endTime);
			}
		} else {
			flg = 1;
		}
	}
	buzzer = 0;
	exploded();
}

void planting(void) {
	xdata char password[] = "7355608_";
	unsigned long startTime = cnt1ms, endTime = cnt1ms + 1000 * plantTime;
	memset(printStr, 0, sizeof printStr);
	while (cnt1ms < endTime) {
		printStr[(cnt1ms - startTime) / 125 / plantTime] = password[(cnt1ms - startTime) / 125 / plantTime];
		printStr[(cnt1ms - startTime) / 125 / plantTime + 1] = '_';
		
		setFreq(-10);
		if ((cnt1ms - startTime) % 500 < 100) {
			TR0 = 1;
		} else {
			TR0 = 0;
		}
		
		print();
		if (button == 1) {
			delayms(50);
			if (button == 1) {
				TR0 = 0;
				return ;
			}
		}
	}
	TR0 = 0;
	planted();
}

void standby(void) {
	while (1) {
		strcpy(printStr, " standby");
		print();
		if (button == 0) {
			delayms(50);
			planting();
		}
	}
}

void main(void) {
    TMOD = 0x10; 
	EA = ET1 = 1;
	TH1 = 0xFC;
	TL1 = 0x18;
	TR1 = 1;
	button = 1;
	beep(0, 0);
	standby();
}
