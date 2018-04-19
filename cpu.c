#include <stdio.h>

#include "cpu.h"
#include "mmu.h"
#include "interrupts.h"
#include "instructions.h"
#include "lcd.h"

void update(FILE *fp, CPU *cpu){
    const int MAXCYCLES = 69905;
    int cyclesThisUpdate = 0;

    while(cyclesThisUpdate < MAXCYCLES){
		int cycles;
        if(cpu->halted){
			cycles = 4;	
        }else{
        	cycles = execute_opcode(fp, cpu);
        	if(cycles == -1){
            	fprintf(fp, "Error\n");
        	}
        }
        cyclesThisUpdate+=cycles;
        UpdateTimers(cpu->mmu->timer, cycles);
        UpdateGraphics(cpu->mmu, cycles);
//        printf("CPU LCD SC_L: %d\n", cpu->mmu->lcd->sc_l);
        DoInterrupts(cpu);
    }

    Render_Screen(fp, cpu->mmu->lcd);
}

void init(CPU *cpu, char *filename){
	cpu->halted = 0;
    cpu->PC.val = 0x0100;
    cpu->AF.val = 0x01B0;
    cpu->BC.val = 0x0013;
    cpu->DE.val = 0x00D8;
    cpu->HL.val = 0x014D;
    cpu->SP.val = 0xFFFE;
    cpu->mmu = mmu_init(filename);
}

void print_cpu(FILE *fp, CPU *cpu){
    fprintf(fp, "AF: 0x%04x ", cpu->AF.val);
    fprintf(fp, "BC: 0x%04x\n", cpu->BC.val);
    fprintf(fp, "DE: 0x%04x ", cpu->DE.val);
    fprintf(fp, "HL: 0x%04x\n", cpu->HL.val);
    fprintf(fp, "SP: 0x%04x ", cpu->SP.val);
    fprintf(fp, "PC: 0x%04x\n", cpu->PC.val);
}
