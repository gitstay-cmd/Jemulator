#ifndef cpu_h_INCLUDED
#define cpu_h_INCLUDED

#include "includes.h"
#include "registers.h"
#include <stdio.h>

typedef struct mmu_S MMU;
typedef struct cpu_S {
    BYTE halted;
    MMU *mmu;
    register_u AF;
    register_u BC;
    register_u DE;
    register_u HL;
    register_u SP;
    register_u PC;
}CPU;

void print_cpu(FILE *fp, CPU *cpu);
void init(CPU *cpu, char *filename);
void update(FILE *fp, CPU *cpu);
#endif // cpu_h_INCLUDED

