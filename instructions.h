#ifndef instructions_h_INCLUDED
#define instructions_h_INCLUDED
#include "cpu.h"

struct instruction{
    char *instruction;
    int (*funct)(CPU *cpu);
};

extern struct instruction instructions[256];
extern struct instruction extendedInstructions[256];
int NOP(CPU *cpu);
int execute_opcode(FILE *fp, CPU *cpu);
#endif // instructions_h_INCLUDED

