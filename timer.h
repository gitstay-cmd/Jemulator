#ifndef timer_h_INCLUDED
#define timer_h_INCLUDED

#include "includes.h"

#define TIMA 0xFF05
#define TMA 0xFF06
#define TMC 0xFF07

#define CLOCKSPEED 4194304

typedef struct timer_S{
	int counter;
	BYTE en;
	BYTE tima;
	BYTE tma;
	BYTE d_r;
	SHORT d_c;
	union {
    	struct{
        	BYTE one: 1;
        	BYTE two: 1;
        	BYTE three: 1;
        	BYTE unused: 5;
    	};
    	BYTE tmc;
	};
}TIMER;

void SetClockFreq(TIMER *timer);
void timer_init(TIMER *timer);
BYTE GetClockFreq(TIMER *timer);
void UpdateTimers(TIMER *timer, int cycles);
#endif // timer_h_INCLUDED

