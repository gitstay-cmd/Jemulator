#ifndef interrupts_h_INCLUDED
#define interrupts_h_INCLUDED

#include "includes.h"
#include "cpu.h"

#define VBLANK 	0x01
#define LCDI	0x02
#define TIMERI	0x04
#define JOYPDI	0x08

typedef struct interrupts_S {
    BYTE seriale;
    BYTE master;
    union {
        struct {
            BYTE vblanke: 1;
            BYTE lcde: 1;
            BYTE timere: 1;
            BYTE joypde: 1;
            BYTE notused: 4;
        };
        BYTE enabled;
    };
    union {
        struct {
            BYTE vblankr: 1;
            BYTE lcdr: 1;
            BYTE timerr: 1;
            BYTE joypdr: 1;
            BYTE notusedr: 4;
        };
        BYTE req;
    };
}interrupts;

interrupts interrupt_C;

void RequestInterrupt(int id);
void ServiceInterrupt(CPU *cpu, int interrupt);
void DoInterrupts(CPU *cpu);
BYTE interrupt_read(SHORT address);
void interrupt_write(SHORT address, BYTE value);
void reset();
#endif // interrupts_h_INCLUDED

