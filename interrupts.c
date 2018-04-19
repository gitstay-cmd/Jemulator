#include <stdio.h>
#include "interrupts.h"
#include "includes.h"
#include "mmu.h"
#include "cpu.h"
#include "registers.h"

void reset(){
    interrupt_C.master = 0;
    interrupt_C.enabled = 0;
    interrupt_C.req = 0;
}

void RequestInterrupt(int id){
	switch(id){
    	case 0:
        	interrupt_C.vblanke = 1;
        	break;
       	case 1:
           	interrupt_C.lcde = 1;
           	break;
        case 2:
            interrupt_C.timere = 1;
            break;
        case 3:
            interrupt_C.seriale = 1;
            break;
        case 4:
            interrupt_C.joypde = 1;
            break;
        default:
            break;
	}
}

void DoInterrupts(CPU *cpu){
    if(interrupt_C.master){

        if(interrupt_C.req > 0){

            for(int i = 0; i < 5; i++){

                if(interrupt_C.req & (1 << i)){

                    if(interrupt_C.enabled & (1 << i)) {

                        ServiceInterrupt(cpu, i);

                    }
                }
            }
        }
    }
}

BYTE interrupt_read(SHORT address){
    switch(address){
        case 0xFF0F:
            return interrupt_C.req;
        case 0xFFFF:
            return interrupt_C.enabled;
        default:
            return 0;
    }
}
void interrupt_write(SHORT address, BYTE value){
    switch(address) {
        case 0xFF0F:
            interrupt_C.req = value;
            break;
        case 0xFFFF:
            interrupt_C.enabled = value;
            break;
        default: break;
    }
}

void ServiceInterrupt(CPU *cpu, int interrupt){
   	interrupt_C.master = 0;
	interrupt_C.req &= ~(1 << interrupt);
    cpu->halted = 0;
    _push(cpu, cpu->PC);

    switch(interrupt){
        case 0: cpu->PC.val = 0x40; break;
        case 1:	cpu->PC.val = 0x48; break;
        case 2: cpu->PC.val = 0x50; break;
        case 3: cpu->PC.val = 0x58; break;
        case 4: cpu->PC.val = 0x60; break;
        default:
            break;
    }
}
