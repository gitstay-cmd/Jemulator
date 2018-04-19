#include <stdio.h>
#include "timer.h"
#include "mmu.h"
#include "includes.h"
#include "interrupts.h"

void timer_init(TIMER *timer){
    timer->counter = CLOCKSPEED/4096;
    timer->en = 1;
    timer->tima = 0;
    timer->tma = 0;
    timer->d_r = 0;
    timer->tmc = 0;
    timer->d_c = 0;
}

void divider_update(TIMER *timer, int cycles){
    timer->d_c += cycles;
    if(timer->d_c >= 255){
        timer->d_c = 0;
        timer->d_r++;
    }
}

BYTE GetClockFreq(TIMER *timer) {
    BYTE freq = 0;
    freq |= ((timer->two << 1) | timer->one);
    return freq;
}

void SetClockFreq(TIMER *timer) {
    BYTE freq = GetClockFreq(timer);
	switch(freq){
    	case 0: timer->counter = 1024; break; // freq 4096
    	case 1: timer->counter = 16; break; //freq 262144
    	case 2: timer->counter = 64; break; //freq 65536
    	case 3: timer->counter = 256; break; //freq 16382
        default:break;
	}
}

BYTE IsClockEnabled(TIMER *timer){
	return timer->three;    
}

void UpdateTimers(TIMER *timer, int cycles){
    divider_update(timer, cycles);

    if(timer->en){
        timer->counter -= cycles;

        if(timer->counter <= 0){
            SetClockFreq(timer);
        }

        if(timer->tima == 255){
            timer->tima = timer->tma;
            RequestInterrupt(2);
        }else{
            timer->tima++;
        }
    }
}
