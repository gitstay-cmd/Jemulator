#include <stdio.h>
#include "includes.h"
#include "cpu.h"
#include "mmu.h"
#include "instructions.h"
#include "interrupts.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int execute_opcode(FILE *fp, CPU *cpu){
    BYTE opc = read_byte(cpu->mmu, cpu->PC.val);
	print_cpu(fp, cpu);
    cpu->PC.val += 1;
    int cycles = instructions[opc].funct(cpu);
	if(!strcmp(instructions[opc].instruction, "UNKNOWN")){
		fprintf(fp, "Unkown instruction: %x\n PC: %x\n", opc, (cpu->PC.val - 1));
		exit(0);
    }else {
        fprintf(fp, "Opcode: 0x%x, Instruction: %s Scanline: %x Cycles: %x\n",opc, instructions[opc].instruction, cpu->mmu->lcd->sc_l, cycles);
    }
    return cycles;
}

int execute_extended_opcode(CPU *cpu){
    BYTE opc = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    return extendedInstructions[opc].funct(cpu);
}

int NOP(CPU *cpu){
    return 4;
}

int load_b_n(CPU *cpu){
    cpu->BC.hi = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    return 8;
}

int load_c_n(CPU *cpu){
    cpu->BC.lo = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    return 8;
}
int load_d_n(CPU *cpu){
    cpu->DE.hi = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    return 8;
}
int load_e_n(CPU *cpu){
    cpu->DE.lo = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    return 8;
}

int load_h_n(CPU *cpu){
    cpu->HL.hi = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    return 8;
}
int load_l_n(CPU *cpu){
    cpu->HL.lo = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    return 8;
}

int load_bc_a(CPU *cpu){
    write_byte(cpu->mmu, cpu->BC.val, cpu->AF.lo);
	return 8;
}

int inc_bc(CPU *cpu){
    cpu->BC.val += 1;
    return 8;
}

int inc_de(CPU *cpu){
    cpu->DE.val += 1;
    return 8;
}

int inc_hl(CPU *cpu){
    cpu->HL.val += 1;
    return 8;
}

int inc_sp(CPU *cpu){
    cpu->SP.val += 1;
    return 8;
}

int dec_bc(CPU *cpu){
    cpu->BC.val -= 1;
    return 8;
}

int dec_de(CPU *cpu){
    cpu->DE.val -= 1;
    return 8;
}

int dec_hl(CPU *cpu){
    cpu->HL.val -= 1;
    return 8;
}

int dec_sp(CPU *cpu){
    cpu->SP.val -= 1;
    return 8;
}

int load_a_a(CPU *cpu){
    return NOP(cpu);
}

int load_a_b(CPU *cpu){
    cpu->AF.hi = cpu->BC.hi;
    return 4;
}

int load_a_c(CPU *cpu){
    cpu->AF.hi = cpu->BC.lo;
    return 4;
}

int load_a_d(CPU *cpu){
    cpu->AF.hi = cpu->DE.hi;
    return 4;
}

int load_a_e(CPU *cpu){
    cpu->AF.hi = cpu->DE.lo;
    return 4;
}

int load_a_h(CPU *cpu){
    cpu->AF.hi = cpu->HL.hi;
    return 4;
}

int load_a_l(CPU *cpu){
    cpu->AF.hi = cpu->HL.lo;
    return 4;
}

int load_a_hl(CPU *cpu){
    cpu->AF.hi = read_byte(cpu->mmu, cpu->HL.val);
    return 8;
}

int load_a_bc(CPU *cpu){
    cpu->AF.hi = read_byte(cpu->mmu, cpu->BC.val);
    return 8;
}
int load_a_de(CPU *cpu){
    cpu->AF.hi = read_byte(cpu->mmu, cpu->DE.val);
    return 8;
}
int load_a_nn(CPU *cpu){
    SHORT address = (read_byte(cpu->mmu, cpu->PC.val+1) <<8);
    address |= read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 2;
    cpu->AF.hi = read_byte(cpu->mmu, address);
    return 16;
}

int load_a_n(CPU *cpu){
    cpu->AF.hi = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    return 8;
}

int load_b_a(CPU *cpu){
    cpu->BC.hi = cpu->AF.hi;
    return 4;
}

int load_b_b(CPU *cpu){
    return NOP(cpu);
}
int load_b_c(CPU *cpu){
    cpu->BC.hi = cpu->BC.lo;
    return 4;
}

int load_b_d(CPU *cpu){
    cpu->BC.hi = cpu->DE.hi;
    return 4;
}

int load_b_e(CPU *cpu){
    cpu->BC.hi = cpu->DE.lo;
    return 4;
}

int load_b_h(CPU *cpu){
    cpu->BC.hi = cpu->HL.hi;
    return 4;
}

int load_b_l(CPU *cpu){
    cpu->BC.hi = cpu->HL.lo;
    return 4;
}

int load_b_hl(CPU *cpu){
    cpu->BC.hi = read_byte(cpu->mmu, cpu->HL.val);
    return 8;
}


int load_c_a(CPU *cpu){
    cpu->BC.lo = cpu->AF.hi;
    return 4;
}

int load_c_b(CPU *cpu){
    cpu->BC.lo = cpu->BC.hi;
    return 4;
}

int load_c_c(CPU *cpu){
    return NOP(cpu);
}

int load_c_d(CPU *cpu) {
    cpu->BC.lo = cpu->DE.hi;
    return 4;
}

int load_c_e(CPU *cpu){
    cpu->BC.lo = cpu->DE.lo;
    return 4;
}

int load_c_h(CPU *cpu){
    cpu->BC.lo = cpu->HL.hi;
    return 4;
}

int load_c_l(CPU *cpu){
    cpu->BC.lo = cpu->HL.lo;
    return 4;
}

int load_c_hl(CPU *cpu){
    cpu->BC.lo = read_byte(cpu->mmu, cpu->HL.val);
    return 8;
}

int load_d_a(CPU *cpu){
    cpu->DE.hi = cpu->AF.hi;
    return 4;
}

int load_d_b(CPU *cpu){
    cpu->DE.hi = cpu->BC.hi;
    return 4;
}

int load_d_c(CPU *cpu){
    cpu->DE.hi = cpu->BC.lo;
    return 4;
}

int load_d_d(CPU *cpu) {
    return NOP(cpu);
}

int load_d_e(CPU *cpu){
    cpu->DE.hi = cpu->DE.lo;
    return 4;
}

int load_d_h(CPU *cpu){
    cpu->DE.hi = cpu->HL.hi;
    return 4;
}

int load_d_l(CPU *cpu){
    cpu->DE.hi = cpu->HL.lo;
    return 4;
}

int load_d_hl(CPU *cpu){
    cpu->DE.hi = read_byte(cpu->mmu, cpu->HL.val);
    return 8;
}

int load_e_a(CPU *cpu){
    cpu->DE.lo = cpu->AF.hi;
    return 4;
}

int load_e_b(CPU *cpu){
    cpu->DE.lo = cpu->BC.hi;
    return 4;
}

int load_e_c(CPU *cpu){
    cpu->DE.lo = cpu->BC.lo;
    return 4;
}

int load_e_d(CPU *cpu) {
    cpu->DE.lo = cpu->DE.hi;
    return 4;
}

int load_e_e(CPU *cpu){
    return NOP(cpu);
}

int load_e_h(CPU *cpu){
    cpu->DE.lo = cpu->HL.hi;
    return 4;
}

int load_e_l(CPU *cpu){
    cpu->DE.lo = cpu->HL.lo;
    return 4;
}

int load_e_hl(CPU *cpu){
    cpu->DE.lo = read_byte(cpu->mmu, cpu->HL.val);
    return 8;
}

int load_h_a(CPU *cpu){
    cpu->HL.hi = cpu->AF.hi;
    return 4;
}

int load_h_b(CPU *cpu){
    cpu->HL.hi = cpu->BC.hi;
    return 4;
}

int load_h_c(CPU *cpu){
    cpu->HL.hi = cpu->BC.lo;
    return 4;
}

int load_h_d(CPU *cpu) {
    cpu->HL.hi = cpu->DE.hi;
    return 4;
}

int load_h_e(CPU *cpu){
    cpu->HL.hi = cpu->DE.lo;
    return 4;
}

int load_h_h(CPU *cpu){
    return NOP(cpu);
}

int load_h_l(CPU *cpu){
    cpu->HL.hi = cpu->HL.lo;
    return 4;
}

int load_h_hl(CPU *cpu){
    cpu->HL.hi = read_byte(cpu->mmu, cpu->HL.val);
    return 8;
}

int load_l_a(CPU *cpu){
    cpu->HL.lo = cpu->AF.hi;
    return 4;
}

int load_l_b(CPU *cpu){
    cpu->HL.lo = cpu->BC.hi;
    return 4;
}

int load_l_c(CPU *cpu){
    cpu->HL.lo = cpu->BC.hi;
    return 4;
}

int load_l_d(CPU *cpu) {
    cpu->HL.lo = cpu->DE.hi;
    return 4;
}

int load_l_e(CPU *cpu){
    cpu->HL.lo = cpu->DE.lo;
    return 4;
}

int load_l_h(CPU *cpu){
    cpu->HL.lo = cpu->HL.hi;
    return 4;
}

int load_l_l(CPU *cpu) {
    return NOP(cpu);
}

int load_l_hl(CPU *cpu){
    cpu->HL.lo = read_byte(cpu->mmu, cpu->HL.val);
    return 8;
}

int load_hl_a(CPU *cpu) {
    write_byte(cpu->mmu, cpu->HL.val, cpu->AF.hi);
    return 8;
}

int load_de_a(CPU *cpu) {
    write_byte(cpu->mmu, cpu->DE.val, cpu->AF.hi);
    return 8;
}

int load_nn_a(CPU *cpu) {
    SHORT address = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 2;
    write_byte(cpu->mmu, address, cpu->AF.hi);
    return 16;
}

int load_hl_b(CPU *cpu){
    write_byte(cpu->mmu, cpu->HL.val, cpu->BC.hi);
    return 8;
}

int load_hl_c(CPU *cpu){
    write_byte(cpu->mmu, cpu->HL.val, cpu->BC.lo);
    return 8;
}

int load_hl_d(CPU *cpu){
    write_byte(cpu->mmu, cpu->HL.val, cpu->DE.hi);
    return 8;
}
int load_hl_e(CPU *cpu){
    write_byte(cpu->mmu, cpu->HL.val, cpu->DE.lo);
    return 8;
}
int load_hl_h(CPU *cpu){
    write_byte(cpu->mmu, cpu->HL.val, cpu->HL.hi);
    return 8;
}
int load_hl_l(CPU *cpu){
    write_byte(cpu->mmu, cpu->HL.val, cpu->HL.lo);
    return 8;
}
int load_hl_n(CPU *cpu){
    write_byte(cpu->mmu, cpu->HL.val, read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 1;
    return 12;
}

int load_a_cp(CPU *cpu){
    cpu->AF.hi = read_byte(cpu->mmu, (SHORT)(cpu->BC.lo + 0xFF00));
    return 8;
}

int load_cp_a(CPU *cpu){
    write_byte(cpu->mmu, (SHORT)(0xFF00 + cpu->BC.lo), cpu->AF.hi);
    return 8;
}

int loadi_a_hl(CPU *cpu){
    cpu->AF.hi = read_byte(cpu->mmu, cpu->HL.val);
    cpu->HL.val++;
    return 8;
}
int loadi_hl_a(CPU *cpu) {
    write_byte(cpu->mmu, cpu->HL.val, cpu->AF.hi);
    cpu->HL.val++;
    return 8;
}

int load_n_a(CPU *cpu){
    BYTE n = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    write_byte(cpu->mmu, (SHORT)(0xFF00 + n), cpu->AF.hi);
    return 12;
}

int load_a_np(CPU *cpu){
    BYTE n = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    cpu->AF.hi = read_byte(cpu->mmu, (SHORT)(0xFF00 + n));
    return 12;
}

int load_np_a(CPU *cpu){
    BYTE n = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    write_byte(cpu->mmu, 0xFF00 + n, cpu->AF.hi);
    return 12;
}

int load_bc_nn(CPU *cpu){
    SHORT nn = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 2;
    cpu->BC.val = nn;
    return 12;
}

int load_de_nn(CPU *cpu){
    SHORT nn = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
	cpu->PC.val += 2;
    cpu->DE.val = nn;
    return 12;
}
int load_hl_nn(CPU *cpu){
    SHORT nn = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
	cpu->PC.val += 2;
    cpu->HL.val = nn;
    return 12;
}
int load_sp_nn(CPU *cpu){
    SHORT nn = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
	cpu->PC.val += 2;
    cpu->SP.val = nn;
    return 12;
}

int loadd_a_hl(CPU *cpu){
    cpu->AF.hi = read_byte(cpu->mmu, cpu->HL.val);
    cpu->HL.val--;
    return 8;
}

int loadd_hl_a(CPU *cpu) {
    write_byte(cpu->mmu, cpu->HL.val, cpu->AF.hi);
    cpu->HL.val--;
    return 8;
}

int load_sp_hl(CPU *cpu){
    cpu->SP.val = cpu->HL.val;
    return 8;
}

int load_sp_n(CPU *cpu){
    int sum = cpu->SP.val + read_byte(cpu->mmu, cpu->PC.val);
    if(sum & 0xFFFF0000){
        cpu->AF.lo |= CARRY;
    }else {
        cpu->AF.lo &= ~CARRY;
    }

    if((sum & 0x0f) + (cpu->SP.lo & 0x0f) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

	cpu->SP.val = (SHORT)sum;
    cpu->AF.lo &= ~ZERO;
    cpu->AF.lo &= ~OPER;
    return 12;
}

int load_nn_sp(CPU *cpu){
    SHORT nn = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 2;
    write_byte(cpu->mmu, nn, cpu->SP.lo);
    write_byte(cpu->mmu, nn+1, cpu->SP.hi);
    return 20;
}

int push_af(CPU *cpu){
    _push(cpu, cpu->BC);
    return 16;
}

int push_bc(CPU *cpu){
    _push(cpu, cpu->BC);
    return 16;
}

int push_de(CPU *cpu){
    _push(cpu, cpu->DE);
    return 16;
}

int push_hl(CPU *cpu){
    _push(cpu, cpu->HL);
    return 16;
}

int pop_af(CPU *cpu){
    cpu->AF.val = _pop(cpu);
    return 12;
}

int pop_bc(CPU *cpu){
    cpu->BC.val = _pop(cpu);
    return 12;
}

int pop_de(CPU *cpu){
    cpu->DE.val = _pop(cpu);
    return 12;
}

int pop_hl(CPU *cpu){
    cpu->HL.val = _pop(cpu);
    return 12;
}

int add_a_a(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->AF);
    BYTE val = sum_r->hi;
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int add_a_b(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->BC);
    BYTE val = sum_r->hi;
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int add_a_c(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->BC);
    BYTE val = sum_r->lo;
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int add_a_d(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->DE);
    BYTE val = sum_r->hi;
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int add_a_e(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->DE);
    BYTE val = sum_r->lo;
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int add_a_h(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->HL);
    BYTE val = sum_r->hi;
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int add_a_l(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->HL);
    BYTE val = sum_r->lo;
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int add_a_hl(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 8;
}

int add_a_n(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    BYTE val = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    int sum = base->hi + val;
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 8;
}

int adc_a_a(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->AF);
    BYTE val = sum_r->hi;
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int adc_a_b(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->BC);
    BYTE val = sum_r->hi;
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int adc_a_c(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->BC);
    BYTE val = sum_r->lo;
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int adc_a_d(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->DE);
    BYTE val = sum_r->hi;
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int adc_a_e(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->DE);
    BYTE val = sum_r->lo;
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int adc_a_h(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->HL);
    BYTE val = sum_r->hi;
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->lo = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int adc_a_l(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    register_u *sum_r;
    sum_r = &(cpu->HL);
    BYTE val = sum_r->lo;
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 4;
}

int adc_a_hl(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 8;
}

int adc_a_n(CPU *cpu){
    register_u *base;
    base = &(cpu->AF);
    BYTE val = read_byte(cpu->mmu, cpu->PC.val);
    int sum = base->hi + val;
    sum += ((cpu->AF.lo & CARRY)? 1: 0);
    if(!sum ){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
    if(sum & 0xff00){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    base->hi = (BYTE)(sum & 0xff);
    if(((base->hi & 0x0F) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;

    return 8;
}

void sub(CPU *cpu, BYTE val){
    cpu->AF.lo |= OPER;

    if(val > cpu->AF.lo){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    if((val &0x0f) > (cpu->AF.hi &0x0f)){
        cpu->AF.lo |= HALFC;
    }else{
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.hi -= val;

    if(cpu->AF.hi){
        cpu->AF.lo &= ~ZERO;
    }else {
        cpu->AF.lo |= ZERO;
    }

}

int sub_a(CPU *cpu){
    sub(cpu, cpu->AF.hi);
    return 4;
}

int sub_b(CPU *cpu){
    sub(cpu, cpu->BC.hi);
    return 4;
}
int sub_c(CPU *cpu){
    sub(cpu, cpu->BC.lo);
    return 4;
}
int sub_d(CPU *cpu){
    sub(cpu, cpu->DE.hi);
    return 4;
}
int sub_e(CPU *cpu){
    sub(cpu, cpu->DE.lo);
    return 4;
}
int sub_h(CPU *cpu){
    sub(cpu, cpu->HL.hi);
    return 4;
}
int sub_l(CPU *cpu){
    sub(cpu, cpu->HL.lo);
    return 4;
}

int sub_hl(CPU *cpu){
    sub(cpu, read_byte(cpu->mmu, cpu->HL.val));
    return 8;
}

int sub_nn(CPU *cpu){
    sub(cpu, read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 1;
    return 8;
}

void cp(CPU *cpu, BYTE val){
    cpu->AF.lo |= OPER;

    if(val > cpu->AF.hi){
        cpu->AF.lo |= CARRY;
    }else{
        cpu->AF.lo &= ~CARRY;
    }
    if((val &0x0f) > (cpu->AF.hi &0x0f)){
        cpu->AF.lo |= HALFC;
    }else{
        cpu->AF.lo &= ~HALFC;
    }

    BYTE value = cpu->AF.hi - val;

    if(value){
        cpu->AF.lo &= ~ZERO;
    }else {
        cpu->AF.lo |= ZERO;
    }

}

int cp_a(CPU *cpu){
    cp(cpu, cpu->AF.hi);
    return 4;
}

int cp_b(CPU *cpu){
    cp(cpu, cpu->BC.hi);
    return 4;
}
int cp_c(CPU *cpu){
    cp(cpu, cpu->BC.lo);
    return 4;
}
int cp_d(CPU *cpu){
    cp(cpu, cpu->DE.hi);
    return 4;
}
int cp_e(CPU *cpu){
    cp(cpu, cpu->DE.lo);
    return 4;
}
int cp_h(CPU *cpu){
    cp(cpu, cpu->HL.hi);
    return 4;
}
int cp_l(CPU *cpu){
    cp(cpu, cpu->HL.lo);
    return 4;
}

int cp_hl(CPU *cpu){
    cp(cpu, read_byte(cpu->mmu, cpu->HL.val));
    return 8;
}

int cp_nn(CPU *cpu){
    cp(cpu, read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 1;
    return 8;
}

void subc(CPU *cpu, BYTE val){
    val += is_C(cpu->AF);
    cpu->AF.lo |= OPER;
    if(val > cpu->AF.hi){
        cpu->AF.lo |= CARRY;
    }else {
        cpu->AF.lo &= ~CARRY;
    }
    if(val == cpu->AF.hi){
        cpu->AF.lo |= ZERO;
    }else{
        cpu->AF.lo &= ~ZERO;
    }

    if((val & 0x0f)> (cpu->AF.hi & 0x0f)){
        cpu->AF.lo |= HALFC;
    }else{
        cpu->AF.lo &= ~HALFC;
    }
    cpu->AF.hi -= val;
}

int subc_a(CPU *cpu){
    subc(cpu, cpu->AF.hi);
    return 4;
}

int subc_b(CPU *cpu){
    subc(cpu, cpu->BC.hi);
    return 4;
}
int subc_c(CPU *cpu){
    subc(cpu, cpu->BC.lo);
    return 4;
}
int subc_d(CPU *cpu){
    subc(cpu, cpu->DE.hi);
    return 4;
}
int subc_e(CPU *cpu){
    subc(cpu, cpu->DE.lo);
    return 4;
}
int subc_h(CPU *cpu){
    subc(cpu, cpu->HL.hi);
    return 4;
}
int subc_l(CPU *cpu){
    subc(cpu, cpu->HL.lo);
    return 4;
}

int subc_hl(CPU *cpu){
    subc(cpu, read_byte(cpu->mmu, cpu->HL.val));
    return 8;
}

int subc_nn(CPU *cpu){
    subc(cpu, read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 1;
    return 8;
}

void and(CPU *cpu, BYTE value){
    cpu->AF.lo &= ~OPER;
    cpu->AF.lo &= ~CARRY;
    cpu->AF.lo |= HALFC;
    cpu->AF.hi &= value;
    if(!cpu->AF.hi){
        cpu->AF.lo |= ZERO;
    }else{
        cpu->AF.lo &= ~ZERO;
    }
}

int and_a(CPU *cpu){
    and(cpu, cpu->AF.hi);
    return 4;
}

int and_b(CPU *cpu){
    and(cpu, cpu->BC.hi);
    return 4;
}
int and_c(CPU *cpu){
    and(cpu, cpu->BC.lo);
    return 4;
}
int and_d(CPU *cpu){
    and(cpu, cpu->DE.hi);
    return 4;
}
int and_e(CPU *cpu){
    and(cpu, cpu->DE.lo);
    return 4;
}
int and_h(CPU *cpu){
    and(cpu, cpu->HL.hi);
    return 4;
}
int and_l(CPU *cpu){
    and(cpu, cpu->HL.lo);
    return 4;
}

int and_hl(CPU *cpu){
    and(cpu, read_byte(cpu->mmu, cpu->HL.val));
    return 8;
}

int and_nn(CPU *cpu){
    and(cpu, read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 1;
    return 8;
}

void or(CPU *cpu, BYTE value){
    cpu->AF.lo &= ~OPER;
    cpu->AF.lo &= ~CARRY;
    cpu->AF.lo &= ~HALFC;
    cpu->AF.hi |= value;
    if(!cpu->AF.hi){
        cpu->AF.lo |= ZERO;
    }else{
        cpu->AF.lo &= ~ZERO;
    }
}

int or_a(CPU *cpu){
    or(cpu, cpu->AF.hi);
    return 4;
}

int or_b(CPU *cpu){
    or(cpu, cpu->BC.hi);
    return 4;
}
int or_c(CPU *cpu){
    or(cpu, cpu->BC.lo);
    return 4;
}
int or_d(CPU *cpu){
    or(cpu, cpu->DE.hi);
    return 4;
}
int or_e(CPU *cpu){
    or(cpu, cpu->DE.lo);
    return 4;
}
int or_h(CPU *cpu){
    or(cpu, cpu->HL.hi);
    return 4;
}
int or_l(CPU *cpu){
    or(cpu, cpu->HL.lo);
    return 4;
}
int or_hl(CPU *cpu){
    or(cpu, read_byte(cpu->mmu, cpu->HL.val));
    return 8;
}

int or_nn(CPU *cpu){
    or(cpu, read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 1;
    return 8;
}

void xor(CPU *cpu, BYTE value){
    cpu->AF.lo &= ~OPER;
    cpu->AF.lo &= ~CARRY;
    cpu->AF.lo &= ~HALFC;
    cpu->AF.hi ^= value;
    if(!cpu->AF.hi){
        cpu->AF.lo |= ZERO;
    }else{
        cpu->AF.lo &= ~ZERO;
    }
}

int xor_a(CPU *cpu){
    xor(cpu, cpu->AF.hi);
    return 4;
}

int xor_b(CPU *cpu){
    xor(cpu, cpu->BC.hi);
    return 4;
}
int xor_c(CPU *cpu){
    xor(cpu, cpu->BC.lo);
    return 4;
}
int xor_d(CPU *cpu){
    xor(cpu, cpu->DE.hi);
    return 4;
}
int xor_e(CPU *cpu){
    xor(cpu, cpu->DE.lo);
    return 4;
}
int xor_h(CPU *cpu){
    xor(cpu, cpu->HL.hi);
    return 4;
}
int xor_l(CPU *cpu){
    xor(cpu, cpu->HL.lo);
    return 4;
}

int xor_hl(CPU *cpu){
    xor(cpu, read_byte(cpu->mmu, cpu->HL.val));
    return 8;
}

int xor_nn(CPU *cpu) {
    xor(cpu, read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 1;
    return 8;
}

void inc(CPU *cpu, BYTE *val){
    if((*val & 0x0f) == 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }
    *val += 1;
    cpu->AF.lo &= ~OPER;
    if(!*val){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
}

int inc_a(CPU *cpu){
    inc(cpu, &cpu->AF.hi);
    return 4;
}

int inc_b(CPU *cpu){
    inc(cpu, &cpu->BC.hi);
    return 4;
}
int inc_c(CPU *cpu){
    inc(cpu, &cpu->BC.lo);
    return 4;
}
int inc_d(CPU *cpu){
    inc(cpu, &cpu->DE.hi);
    return 4;
}
int inc_e(CPU *cpu){
    inc(cpu, &cpu->DE.lo);
    return 4;
}
int inc_h(CPU *cpu){
    inc(cpu, &cpu->HL.hi);
    return 4;
}
int inc_l(CPU *cpu){
    inc(cpu, &cpu->HL.lo);
    return 4;
}

int inc_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    inc(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 12;
}

void dec(CPU *cpu, BYTE *val){
    if((*val & 0x0f)){
        cpu->AF.lo &= ~HALFC;
    }else {
        cpu->AF.lo |= HALFC;
    }
    *val -= 1;
    cpu->AF.lo |= OPER;
    if(*val == 0){
        cpu->AF.lo |= ZERO;
    }else {
        cpu->AF.lo &= ~ZERO;
    }
}

int dec_a(CPU *cpu){
    dec(cpu, &cpu->AF.hi);
    return 4;
}

int dec_b(CPU *cpu){
    dec(cpu, &cpu->BC.hi);
    return 4;
}
int dec_c(CPU *cpu){
    dec(cpu, &cpu->BC.lo);
    return 4;
}
int dec_d(CPU *cpu){
    dec(cpu, &cpu->DE.hi);
    return 4;
}
int dec_e(CPU *cpu){
    dec(cpu, &cpu->DE.lo);
    return 4;
}
int dec_h(CPU *cpu){
    dec(cpu, &cpu->HL.hi);
    return 4;
}
int dec_l(CPU *cpu){
    dec(cpu, &cpu->HL.lo);
    return 4;
}

int dec_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    dec(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 12;
}

void add16(CPU *cpu, register_u *AF, register_u *BC){
    int sum = AF->val + BC->val;
    if(sum & 0xffff0000){
        cpu->AF.lo |= CARRY;
    }else {
        cpu->AF.lo &= ~CARRY;
    }

    AF->val = (SHORT)(sum & 0xffff);
    if(((AF->val & 0x0F) + (BC->val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else {
        cpu->AF.lo &= ~HALFC;
    }

    cpu->AF.lo &= ~OPER;
}

int add_hl_bc(CPU *cpu){
    add16(cpu, &cpu->HL, &cpu->BC);
    return 8;
}

int add_hl_de(CPU *cpu){
    add16(cpu, &cpu->HL, &cpu->DE);
    return 8;
}

int add_hl_hl(CPU *cpu){
    add16(cpu, &cpu->HL, &cpu->HL);
    return 8;
}

int add_hl_sp(CPU *cpu){
    add16(cpu, &cpu->HL, &cpu->SP);
    return 8;
}

int add_sp_n(CPU *cpu){
    cpu->AF.lo &= ~ZERO;
    cpu->AF.lo &= ~OPER;
    BYTE val = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    if(((cpu->SP.val & 0x0f) + (val & 0x0f)) > 0x0f){
        cpu->AF.lo |= HALFC;
    }else{
        cpu->AF.lo &= ~HALFC;
    }
    int sum = cpu->SP.val + val;
    if((sum & 0xFFFF0000)){
        cpu->AF.lo |= CARRY;
    }else {
        cpu->AF.lo &= ~CARRY;
    }
    cpu->SP.val = (SHORT)(sum & 0xFFFF);
    return 16;
}

void swap(CPU *cpu, BYTE *val){
    cpu->AF.lo &= ~OPER;
    cpu->AF.lo &= ~HALFC;
    cpu->AF.lo &= ~CARRY;
    BYTE hold = (*val >> 4);
    hold |= (*val << 4);
    if(!hold){
        cpu->AF.lo |= ZERO;
    }else{
        cpu->AF.lo &= ~ZERO;
    }
    *val = hold;
}

int swap_a(CPU *cpu){
    swap(cpu, &cpu->AF.hi);
    return 8;
}

int swap_b(CPU *cpu){
    swap(cpu, &cpu->BC.hi);
    return 8;
}
int swap_c(CPU *cpu){
    swap(cpu, &cpu->BC.lo);
    return 8;
}
int swap_d(CPU *cpu){
    swap(cpu, &cpu->DE.hi);
    return 8;
}
int swap_e(CPU *cpu){
    swap(cpu, &cpu->DE.lo);
    return 8;
}
int swap_h(CPU *cpu){
    swap(cpu, &cpu->HL.hi);
    return 8;
}
int swap_l(CPU *cpu){
    swap(cpu, &cpu->HL.lo);
    return 8;
}

int swap_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    swap(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int daa(CPU *cpu) {


    {
        unsigned short s = cpu->AF.hi;

        if(is_O(cpu->AF)) {
            if(is_H(cpu->AF)) s = (s - 0x06)&0xFF;
            if(is_C(cpu->AF)) s -= 0x60;
        }
        else {
            if(is_H(cpu->AF) || (s & 0xF) > 9) s += 0x06;
            if(is_H(cpu->AF) || s > 0x9F) s += 0x60;
        }

        cpu->AF.hi = s;
        cpu->AF.lo &= ~HALFC;

        if(cpu->AF.hi) cpu->AF.lo &= ~ZERO;
        else cpu->AF.lo |= ZERO;

        if(s >= 0x100) cpu->AF.lo |= CARRY;
    }
    return 4;

}

int cpl(CPU *cpu){

    cpu->AF.hi ^= 0xFF;
    cpu->AF.lo |= HALFC;
    cpu->AF.lo |= OPER;
    return 4;
}

int ccf(CPU *cpu){
    cpu->AF.lo &= ~(OPER | HALFC);
    cpu->AF.lo ^= CARRY;
    return 4;
}

int scf(CPU *cpu){
    cpu->AF.lo &= ~(OPER| HALFC);
    cpu->AF.lo |= CARRY;
    return 4;
}

int halt(CPU *cpu){
    cpu->halted = 1;
    return 4;
}

int stop(CPU *cpu){
    return 4;
}

int di(CPU *cpu) {
    interrupt_C.enabled = 0;
    return 4;
}

int ei(CPU *cpu){
    interrupt_C.enabled = 1;
    return 4;
}

int rlca(CPU *cpu) {
    BYTE carry = (BYTE)(cpu->AF.hi & 0x80) >> 7;
    if(carry) cpu->AF.lo |= CARRY;
    else cpu->AF.lo &= ~CARRY;

    cpu->AF.hi <<= 1;
    cpu->AF.hi += carry;

    cpu->AF.lo &= ~(OPER | ZERO | HALFC);
    return 4;
}

int rla(CPU *cpu) {
    int carry = is_C(cpu->AF);

    if(cpu->AF.hi & 0x80) cpu->AF.lo |= ~(CARRY);
    else cpu->AF.lo &= ~CARRY;

    cpu->AF.hi <<= 1;
    cpu->AF.hi += carry;

    cpu->AF.lo &= ~(OPER | ZERO | HALFC);
    return 4;
}

int rrca(CPU *cpu) {
    unsigned char carry = cpu->AF.hi & 0x01;
    if(carry) cpu->AF.lo |= (CARRY);
    else cpu->AF.lo &= (CARRY);

    cpu->AF.hi >>= 1;
    if(carry) cpu->AF.hi |= 0x80;

    cpu->AF.lo &= ~(OPER | ZERO | HALFC);
    return 4;
}

void rrc(CPU *cpu, BYTE *value){
    unsigned char carry = *value & 0x01;
    if(carry) cpu->AF.lo |= (CARRY);
    else cpu->AF.lo &= (CARRY);

    *value >>= 1;
    if(carry) *value |= 0x80;

    cpu->AF.lo &= ~(OPER | ZERO | HALFC);
}

int rrc_a(CPU *cpu){
    rrc(cpu, &cpu->AF.hi);
    return 8;
}

int rrc_b(CPU *cpu){
    rrc(cpu, &cpu->BC.hi);
    return 8;
}
int rrc_c(CPU *cpu){
    rrc(cpu, &cpu->BC.lo);
    return 8;
}
int rrc_d(CPU *cpu){
    rrc(cpu, &cpu->DE.hi);
    return 8;
}
int rrc_e(CPU *cpu){
    rrc(cpu, &cpu->DE.lo);
    return 8;
}
int rrc_h(CPU *cpu){
    rrc(cpu, &cpu->HL.hi);
    return 8;
}
int rrc_l(CPU *cpu){
    rrc(cpu, &cpu->HL.lo);
    return 8;
}

int rrc_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    rrc(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int rra(CPU *cpu){
    int carry = is_C(cpu->AF) << 7;

    if(cpu->AF.hi & 0x01) cpu->AF.lo |= (CARRY);
    else cpu->AF.lo &= ~CARRY;

    cpu->AF.hi >>= 1;
    cpu->AF.hi += carry;

    cpu->AF.lo &= ~(OPER | ZERO | HALFC);
    return 4;
}

void rr(CPU *cpu, BYTE *value) {
    int carry = is_C(cpu->AF) << 7;

    if (*value & 0x01) cpu->AF.lo |= (CARRY);
    else cpu->AF.lo &= ~CARRY;

    *value >>= 1;
    *value += carry;

    cpu->AF.lo &= ~(OPER | ZERO | HALFC);
}

int rr_a(CPU *cpu){
    rr(cpu, &cpu->AF.hi);
    return 8;
}

int rr_b(CPU *cpu){
    rr(cpu, &cpu->BC.hi);
    return 8;
}
int rr_c(CPU *cpu){
    rr(cpu, &cpu->BC.lo);
    return 8;
}
int rr_d(CPU *cpu){
    rr(cpu, &cpu->DE.hi);
    return 8;
}
int rr_e(CPU *cpu){
    rr(cpu, &cpu->DE.lo);
    return 8;
}
int rr_h(CPU *cpu){
    rr(cpu, &cpu->HL.hi);
    return 8;
}
int rr_l(CPU *cpu){
    rr(cpu, &cpu->HL.lo);
    return 8;
}

int rr_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    rr(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

void rlc(CPU *cpu, BYTE *value) {
    int carry = (*value & 0x80) >> 7;

    if(*value & 0x80) cpu->AF.lo |= (CARRY);
    else cpu->AF.lo &= ~(CARRY);

    *value <<= 1;
    *value += carry;

    if(*value) cpu->AF.lo &= ~(ZERO);
    else cpu->AF.lo |= (ZERO);

    cpu->AF.lo &= ~(OPER | HALFC);

}

int rlc_a(CPU *cpu){
    rlc(cpu, &cpu->AF.hi);
    return 8;
}

int rlc_b(CPU *cpu){
    rlc(cpu, &cpu->BC.hi);
    return 8;
}
int rlc_c(CPU *cpu){
    rlc(cpu, &cpu->BC.lo);
    return 8;
}
int rlc_d(CPU *cpu){
    rlc(cpu, &cpu->DE.hi);
    return 8;
}
int rlc_e(CPU *cpu){
    rlc(cpu, &cpu->DE.lo);
    return 8;
}
int rlc_h(CPU *cpu){
    rlc(cpu, &cpu->HL.hi);
    return 8;
}
int rlc_l(CPU *cpu){
    rlc(cpu, &cpu->HL.lo);
    return 8;
}

int rlc_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    rlc(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

void rl(CPU *cpu, BYTE *value) {
    int carry = is_C(cpu->AF);

    if(*value & 0x80) cpu->AF.lo |= (CARRY);
    else cpu->AF.lo &= ~(CARRY);

    *value <<= 1;
    *value += carry;

    if(*value) cpu->AF.lo &= ~(ZERO);
    else cpu->AF.lo |= (ZERO);

    cpu->AF.lo &= ~(OPER | HALFC);

}

int rl_a(CPU *cpu){
    rl(cpu, &cpu->AF.hi);
    return 8;
}

int rl_b(CPU *cpu){
    rl(cpu, &cpu->BC.hi);
    return 8;
}
int rl_c(CPU *cpu){
    rl(cpu, &cpu->BC.lo);
    return 8;
}
int rl_d(CPU *cpu){
    rl(cpu, &cpu->DE.hi);
    return 8;
}
int rl_e(CPU *cpu){
    rl(cpu, &cpu->DE.lo);
    return 8;
}
int rl_h(CPU *cpu){
    rl(cpu, &cpu->HL.hi);
    return 8;
}
int rl_l(CPU *cpu){
    rl(cpu, &cpu->HL.lo);
    return 8;
}

int rl_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    rlc(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int reti(CPU *cpu){
    cpu->PC.val = _pop(cpu);
    ei(cpu);
    return 8;
}

int ret_c(CPU *cpu) {
    if(is_C(cpu->AF)) {
        cpu->PC.val = _pop(cpu);
        return 20;
    }
    return 8;
}
int ret_nc(CPU *cpu) {
    if(!is_C(cpu->AF)) {
        cpu->PC.val = _pop(cpu);
        return 20;
    }
    return 8;
}
int ret_nz(CPU *cpu) {
    if(!is_Z(cpu->AF)) {
        cpu->PC.val = _pop(cpu);
        return 20;
    }
    return 8;
}

int ret_z(CPU *cpu) {
    if(is_Z(cpu->AF)) {
        cpu->PC.val = _pop(cpu);
        return 20;
    }
    return 8;
}

int ret(CPU *cpu){
    cpu->PC.val = _pop(cpu);
    return 8;
}

int call_nz_nn(CPU *cpu){
    SHORT address = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 2;
    if(is_Z(cpu->AF)){
		return 12;
    }else{
        _push(cpu, cpu->PC);
        cpu->PC.val = address;
        return 24;
    }
    return 0;
}

int call_z_nn(CPU *cpu){
    SHORT address = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 2;
    if(!is_Z(cpu->AF)){
        return 12;
    }else{
        _push(cpu, cpu->PC);
        cpu->PC.val = address;
        return 24;
    }
    return 0;
}
int call_nn(CPU *cpu){
    SHORT address = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 2;
    _push(cpu, cpu->PC);
    cpu->PC.val = address;
    return 24;
}
int call_nc_nn(CPU *cpu){
    SHORT address = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 2;
    if(is_C(cpu->AF)){
		return 12;
    }else{
        _push(cpu, cpu->PC);
        cpu->PC.val = address;
        return 24;
    }
    return 0;
}

int call_c_nn(CPU *cpu){
    SHORT address = (read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val));
    cpu->PC.val += 2;
    if(!is_C(cpu->AF)){
		return 12;
    }else{
        _push(cpu, cpu->PC);
        cpu->PC.val = address;
        return 24;
    }
    return 0;
}

int jr_n(CPU *cpu) {
    BYTE add = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    cpu->PC.val += (signed char)add;
    return 0;
}

int jr_nz_n(CPU *cpu) {
    BYTE add = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    if(is_Z(cpu->AF)) {return 8;}
    else {
        cpu->PC.val += (signed char)add;
        return 12;
    }
}

int jr_nc_n(CPU *cpu) {
    BYTE add = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    if(is_C(cpu->AF)) {return 8;}
    else {
        cpu->PC.val += (signed char)add;
        return 12;
    }
}

int jr_c_n(CPU *cpu) {
    BYTE add = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    if(!is_C(cpu->AF)){return 8;}
    else {
        cpu->PC.val += (signed char)add;
        return 12;
    }
}

int jr_z_n(CPU *cpu) {
    BYTE add = read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val += 1;
    if(!is_Z(cpu->AF)){ return 8;}
    else {
        cpu->PC.val += (signed char)add;
        return 12;
    }
}


void sla(CPU *cpu, BYTE *value) {
    if(*value & 0x80) cpu->AF.lo |= CARRY;
    else cpu->AF.lo &= ~CARRY;

    *value <<= 1;

    if(*value) cpu->AF.lo &= ~ZERO;
    else cpu->AF.lo |= (ZERO);

    cpu->AF.lo &= ~(OPER | HALFC);
}

int sla_a(CPU *cpu){
    sla(cpu, &cpu->AF.hi);
    return 8;
}

int sla_b(CPU *cpu){
    sla(cpu, &cpu->BC.hi);
    return 8;
}
int sla_c(CPU *cpu){
    sla(cpu, &cpu->BC.lo);
    return 8;
}
int sla_d(CPU *cpu){
    sla(cpu, &cpu->DE.hi);
    return 8;
}
int sla_e(CPU *cpu){
    sla(cpu, &cpu->DE.lo);
    return 8;
}
int sla_h(CPU *cpu){
    sla(cpu, &cpu->HL.hi);
    return 8;
}
int sla_l(CPU *cpu){
    sla(cpu, &cpu->HL.lo);
    return 8;
}

int sla_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    sla(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}


void sra(CPU *cpu, BYTE *value) {
    if(*value & 0x01) cpu->AF.lo |= CARRY;
    else cpu->AF.lo &= ~(CARRY);

    *value = (*value & 0x80) | (*value >> 1);

    if(*value) cpu->AF.lo &= ~(ZERO);
    else cpu->AF.lo |= ZERO;

    cpu->AF.lo &= ~(OPER | HALFC);
}

int sra_a(CPU *cpu){
    sra(cpu, &cpu->AF.hi);
    return 8;
}

int sra_b(CPU *cpu){
    sra(cpu, &cpu->BC.hi);
    return 8;
}
int sra_c(CPU *cpu){
    sra(cpu, &cpu->BC.lo);
    return 8;
}
int sra_d(CPU *cpu){
    sra(cpu, &cpu->DE.hi);
    return 8;
}
int sra_e(CPU *cpu){
    sra(cpu, &cpu->DE.lo);
    return 8;
}
int sra_h(CPU *cpu){
    sra(cpu, &cpu->HL.hi);
    return 8;
}
int sra_l(CPU *cpu){
    sra(cpu, &cpu->HL.lo);
    return 8;
}

int sra_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    sra(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

void srl(CPU *cpu, BYTE *value) {
    if(*value & 0x01) cpu->AF.lo |= CARRY;
    else cpu->AF.lo &= ~(CARRY);

    *value >>= 1;

    if(*value) cpu->AF.lo &= ~(ZERO);
    else cpu->AF.lo |= ZERO;

    cpu->AF.lo &= ~(OPER | HALFC);
}

int srl_a(CPU *cpu){
    srl(cpu, &cpu->AF.hi);
    return 8;
}

int srl_b(CPU *cpu){
    srl(cpu, &cpu->BC.hi);
    return 8;
}
int srl_c(CPU *cpu){
    srl(cpu, &cpu->BC.lo);
    return 8;
}
int srl_d(CPU *cpu){
    srl(cpu, &cpu->DE.hi);
    return 8;
}
int srl_e(CPU *cpu){
    srl(cpu, &cpu->DE.lo);
    return 8;
}
int srl_h(CPU *cpu){
    srl(cpu, &cpu->HL.hi);
    return 8;
}
int srl_l(CPU *cpu){
    srl(cpu, &cpu->HL.lo);
    return 8;
}

int srl_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    srl(cpu, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

void bit(CPU *cpu, BYTE bit, BYTE *val) {
    BYTE value = *val;
    if(value & bit) cpu->AF.lo &= ~ZERO;
    else cpu->AF.lo |= ZERO;

    cpu->AF.lo &= ~OPER;
    cpu->AF.lo |= HALFC;
}

int bit_0_a(CPU *cpu){
    bit(cpu,0, &cpu->AF.hi);
    return 8;
}

int bit_0_b(CPU *cpu){
    bit(cpu,0,&cpu->BC.hi);
    return 8;
}
int bit_0_c(CPU *cpu){
    bit(cpu,0,&cpu->BC.lo);
    return 8;
}
int bit_0_d(CPU *cpu){
    bit(cpu,0,&cpu->DE.hi);
    return 8;
}
int bit_0_e(CPU *cpu){
    bit(cpu,0,&cpu->DE.lo);
    return 8;
}
int bit_0_h(CPU *cpu){
    bit(cpu,0,&cpu->HL.hi);
    return 8;
}
int bit_0_l(CPU *cpu){
    bit(cpu,0,&cpu->HL.lo);
    return 8;
}

int bit_0_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    bit(cpu,0, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int bit_1_a(CPU *cpu){
    bit(cpu, 1, &cpu->AF.hi);
    return 8;
}

int bit_1_b(CPU *cpu){
    bit(cpu, 1, &cpu->BC.hi);
    return 8;
}
int bit_1_c(CPU *cpu){
    bit(cpu, 1, &cpu->BC.lo);
    return 8;
}
int bit_1_d(CPU *cpu){
    bit(cpu, 1, &cpu->DE.hi);
    return 8;
}
int bit_1_e(CPU *cpu){
    bit(cpu, 1, &cpu->DE.lo);
    return 8;
}
int bit_1_h(CPU *cpu){
    bit(cpu,1, &cpu->HL.hi);
    return 8;
}
int bit_1_l(CPU *cpu){
    bit(cpu, 1,&cpu->HL.lo);
    return 8;
}

int bit_1_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    bit(cpu,1, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int bit_2_a(CPU *cpu){
    bit(cpu, 2, &cpu->AF.hi);
    return 8;
}

int bit_2_b(CPU *cpu){
    bit(cpu, 2, &cpu->BC.hi);
    return 8;
}
int bit_2_c(CPU *cpu){
    bit(cpu, 2, &cpu->BC.lo);
    return 8;
}
int bit_2_d(CPU *cpu){
    bit(cpu, 2, &cpu->DE.hi);
    return 8;
}
int bit_2_e(CPU *cpu){
    bit(cpu, 2, &cpu->DE.lo);
    return 8;
}
int bit_2_h(CPU *cpu){
    bit(cpu,2, &cpu->HL.hi);
    return 8;
}
int bit_2_l(CPU *cpu){
    bit(cpu, 2,&cpu->HL.lo);
    return 8;
}

int bit_2_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    bit(cpu,2, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int bit_3_a(CPU *cpu){
    bit(cpu, 3, &cpu->AF.hi);
    return 8;
}

int bit_3_b(CPU *cpu){
    bit(cpu, 3, &cpu->BC.hi);
    return 8;
}
int bit_3_c(CPU *cpu){
    bit(cpu, 3, &cpu->BC.lo);
    return 8;
}
int bit_3_d(CPU *cpu){
    bit(cpu, 3, &cpu->DE.hi);
    return 8;
}
int bit_3_e(CPU *cpu){
    bit(cpu, 3, &cpu->DE.lo);
    return 8;
}
int bit_3_h(CPU *cpu){
    bit(cpu,3, &cpu->HL.hi);
    return 8;
}
int bit_3_l(CPU *cpu){
    bit(cpu, 3,&cpu->HL.lo);
    return 8;
}

int bit_3_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    bit(cpu,3, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int bit_4_a(CPU *cpu){
    bit(cpu, 4, &cpu->AF.hi);
    return 8;
}

int bit_4_b(CPU *cpu){
    bit(cpu, 4, &cpu->BC.hi);
    return 8;
}
int bit_4_c(CPU *cpu){
    bit(cpu, 4, &cpu->BC.lo);
    return 8;
}
int bit_4_d(CPU *cpu){
    bit(cpu, 4, &cpu->DE.hi);
    return 8;
}
int bit_4_e(CPU *cpu){
    bit(cpu, 4, &cpu->DE.lo);
    return 8;
}
int bit_4_h(CPU *cpu){
    bit(cpu,4, &cpu->HL.hi);
    return 8;
}
int bit_4_l(CPU *cpu){
    bit(cpu, 4,&cpu->HL.lo);
    return 8;
}

int bit_4_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    bit(cpu,4, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int bit_5_a(CPU *cpu){
    bit(cpu, 5, &cpu->AF.hi);
    return 8;
}

int bit_5_b(CPU *cpu){
    bit(cpu, 5, &cpu->BC.hi);
    return 8;
}
int bit_5_c(CPU *cpu){
    bit(cpu, 5, &cpu->BC.lo);
    return 8;
}
int bit_5_d(CPU *cpu){
    bit(cpu, 5, &cpu->DE.hi);
    return 8;
}
int bit_5_e(CPU *cpu){
    bit(cpu, 5, &cpu->DE.lo);
    return 8;
}
int bit_5_h(CPU *cpu){
    bit(cpu,5, &cpu->HL.hi);
    return 8;
}
int bit_5_l(CPU *cpu){
    bit(cpu, 5,&cpu->HL.lo);
    return 8;
}

int bit_5_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    bit(cpu,5, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int bit_6_a(CPU *cpu){
    bit(cpu, 6, &cpu->AF.hi);
    return 8;
}

int bit_6_b(CPU *cpu){
    bit(cpu, 6, &cpu->BC.hi);
    return 8;
}
int bit_6_c(CPU *cpu){
    bit(cpu, 6, &cpu->BC.lo);
    return 8;
}
int bit_6_d(CPU *cpu){
    bit(cpu, 6, &cpu->DE.hi);
    return 8;
}
int bit_6_e(CPU *cpu){
    bit(cpu, 6, &cpu->DE.lo);
    return 8;
}
int bit_6_h(CPU *cpu){
    bit(cpu,6, &cpu->HL.hi);
    return 8;
}
int bit_6_l(CPU *cpu){
    bit(cpu, 6,&cpu->HL.lo);
    return 8;
}

int bit_6_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    bit(cpu,6, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int bit_7_a(CPU *cpu){
    bit(cpu, 7, &cpu->AF.hi);
    return 8;
}

int bit_7_b(CPU *cpu){
    bit(cpu, 7, &cpu->BC.hi);
    return 8;
}
int bit_7_c(CPU *cpu){
    bit(cpu, 7, &cpu->BC.lo);
    return 8;
}
int bit_7_d(CPU *cpu){
    bit(cpu, 7, &cpu->DE.hi);
    return 8;
}
int bit_7_e(CPU *cpu){
    bit(cpu, 7, &cpu->DE.lo);
    return 8;
}
int bit_7_h(CPU *cpu){
    bit(cpu,7, &cpu->HL.hi);
    return 8;
}
int bit_7_l(CPU *cpu){
    bit(cpu, 7,&cpu->HL.lo);
    return 8;
}

int bit_7_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    bit(cpu,7, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

void res(CPU *cpu, BYTE res, BYTE *val) {
    *val &= ~(1 << res);
}

int res_0_a(CPU *cpu){
    res(cpu,0,&cpu->AF.hi);
    return 8;
}

int res_0_b(CPU *cpu){
    res(cpu,0,&cpu->BC.hi);
    return 8;
}
int res_0_c(CPU *cpu){
    res(cpu,0,&cpu->BC.lo);
    return 8;
}
int res_0_d(CPU *cpu){
    res(cpu,0,&cpu->DE.hi);
    return 8;
}
int res_0_e(CPU *cpu){
    res(cpu,0,&cpu->DE.lo);
    return 8;
}
int res_0_h(CPU *cpu){
    res(cpu,0,&cpu->HL.hi);
    return 8;
}
int res_0_l(CPU *cpu){
    res(cpu,0,&cpu->HL.lo);
    return 8;
}

int res_0_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    res(cpu,0,&val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int res_1_a(CPU *cpu){
    res(cpu, 1, &cpu->AF.hi);
    return 8;
}

int res_1_b(CPU *cpu){
    res(cpu, 1, &cpu->BC.hi);
    return 8;
}
int res_1_c(CPU *cpu){
    res(cpu, 1, &cpu->BC.lo);
    return 8;
}
int res_1_d(CPU *cpu){
    res(cpu, 1, &cpu->DE.hi);
    return 8;
}
int res_1_e(CPU *cpu){
    res(cpu, 1, &cpu->DE.lo);
    return 8;
}
int res_1_h(CPU *cpu){
    res(cpu,1, &cpu->HL.hi);
    return 8;
}
int res_1_l(CPU *cpu){
    res(cpu, 1,&cpu->HL.lo);
    return 8;
}

int res_1_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    res(cpu,1, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int res_2_a(CPU *cpu){
    res(cpu, 2, &cpu->AF.hi);
    return 8;
}

int res_2_b(CPU *cpu){
    res(cpu, 2, &cpu->BC.hi);
    return 8;
}
int res_2_c(CPU *cpu){
    res(cpu, 2, &cpu->BC.lo);
    return 8;
}
int res_2_d(CPU *cpu){
    res(cpu, 2, &cpu->DE.hi);
    return 8;
}
int res_2_e(CPU *cpu){
    res(cpu, 2, &cpu->DE.lo);
    return 8;
}
int res_2_h(CPU *cpu){
    res(cpu,2, &cpu->HL.hi);
    return 8;
}
int res_2_l(CPU *cpu){
    res(cpu, 2,&cpu->HL.lo);
    return 8;
}

int res_2_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    res(cpu,2, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int res_3_a(CPU *cpu){
    res(cpu, 3, &cpu->AF.hi);
    return 8;
}

int res_3_b(CPU *cpu){
    res(cpu, 3, &cpu->BC.hi);
    return 8;
}
int res_3_c(CPU *cpu){
    res(cpu, 3, &cpu->BC.lo);
    return 8;
}
int res_3_d(CPU *cpu){
    res(cpu, 3, &cpu->DE.hi);
    return 8;
}
int res_3_e(CPU *cpu){
    res(cpu, 3, &cpu->DE.lo);
    return 8;
}
int res_3_h(CPU *cpu){
    res(cpu,3, &cpu->HL.hi);
    return 8;
}
int res_3_l(CPU *cpu){
    res(cpu, 3,&cpu->HL.lo);
    return 8;
}

int res_3_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    res(cpu,3, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int res_4_a(CPU *cpu){
    res(cpu, 4, &cpu->AF.hi);
    return 8;
}

int res_4_b(CPU *cpu){
    res(cpu, 4, &cpu->BC.hi);
    return 8;
}
int res_4_c(CPU *cpu){
    res(cpu, 4, &cpu->BC.lo);
    return 8;
}
int res_4_d(CPU *cpu){
    res(cpu, 4, &cpu->DE.hi);
    return 8;
}
int res_4_e(CPU *cpu){
    res(cpu, 4, &cpu->DE.lo);
    return 8;
}
int res_4_h(CPU *cpu){
    res(cpu,4, &cpu->HL.hi);
    return 8;
}
int res_4_l(CPU *cpu){
    res(cpu, 4,&cpu->HL.lo);
    return 8;
}

int res_4_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    res(cpu,4, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int res_5_a(CPU *cpu){
    res(cpu, 5, &cpu->AF.hi);
    return 8;
}

int res_5_b(CPU *cpu){
    res(cpu, 5, &cpu->BC.hi);
    return 8;
}
int res_5_c(CPU *cpu){
    res(cpu, 5, &cpu->BC.lo);
    return 8;
}
int res_5_d(CPU *cpu){
    res(cpu, 5, &cpu->DE.hi);
    return 8;
}
int res_5_e(CPU *cpu){
    res(cpu, 5, &cpu->DE.lo);
    return 8;
}
int res_5_h(CPU *cpu){
    res(cpu,5, &cpu->HL.hi);
    return 8;
}
int res_5_l(CPU *cpu){
    res(cpu, 5,&cpu->HL.lo);
    return 8;
}

int res_5_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    res(cpu,5, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int res_6_a(CPU *cpu){
    res(cpu, 6, &cpu->AF.hi);
    return 8;
}

int res_6_b(CPU *cpu){
    res(cpu, 6, &cpu->BC.hi);
    return 8;
}
int res_6_c(CPU *cpu){
    res(cpu, 6, &cpu->BC.lo);
    return 8;
}
int res_6_d(CPU *cpu){
    res(cpu, 6, &cpu->DE.hi);
    return 8;
}
int res_6_e(CPU *cpu){
    res(cpu, 6, &cpu->DE.lo);
    return 8;
}
int res_6_h(CPU *cpu){
    res(cpu,6, &cpu->HL.hi);
    return 8;
}
int res_6_l(CPU *cpu){
    res(cpu, 6,&cpu->HL.lo);
    return 8;
}

int res_6_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    res(cpu,6, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int res_7_a(CPU *cpu){
    res(cpu, 7, &cpu->AF.hi);
    return 8;
}

int res_7_b(CPU *cpu){
    res(cpu, 7, &cpu->BC.hi);
    return 8;
}
int res_7_c(CPU *cpu){
    res(cpu, 7, &cpu->BC.lo);
    return 8;
}
int res_7_d(CPU *cpu){
    res(cpu, 7, &cpu->DE.hi);
    return 8;
}
int res_7_e(CPU *cpu){
    res(cpu, 7, &cpu->DE.lo);
    return 8;
}
int res_7_h(CPU *cpu){
    res(cpu,7, &cpu->HL.hi);
    return 8;
}
int res_7_l(CPU *cpu){
    res(cpu, 7,&cpu->HL.lo);
    return 8;
}

int res_7_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    res(cpu,7, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

void set(CPU *cpu, BYTE set, BYTE *val) {
    *val |= (1 << set);
}

int set_0_a(CPU *cpu){
    set(cpu,0,&cpu->AF.hi);
    return 8;
}

int set_0_b(CPU *cpu){
    set(cpu,0,&cpu->BC.hi);
    return 8;
}
int set_0_c(CPU *cpu){
    set(cpu,0,&cpu->BC.lo);
    return 8;
}
int set_0_d(CPU *cpu){
    set(cpu,0,&cpu->DE.hi);
    return 8;
}
int set_0_e(CPU *cpu){
    set(cpu,0,&cpu->DE.lo);
    return 8;
}
int set_0_h(CPU *cpu){
    set(cpu,0,&cpu->HL.hi);
    return 8;
}
int set_0_l(CPU *cpu){
    set(cpu,0,&cpu->HL.lo);
    return 8;
}

int set_0_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    set(cpu,0,&val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int set_1_a(CPU *cpu){
    set(cpu, 1, &cpu->AF.hi);
    return 8;
}

int set_1_b(CPU *cpu){
    set(cpu, 1, &cpu->BC.hi);
    return 8;
}
int set_1_c(CPU *cpu){
    set(cpu, 1, &cpu->BC.lo);
    return 8;
}
int set_1_d(CPU *cpu){
    set(cpu, 1, &cpu->DE.hi);
    return 8;
}
int set_1_e(CPU *cpu){
    set(cpu, 1, &cpu->DE.lo);
    return 8;
}
int set_1_h(CPU *cpu){
    set(cpu,1, &cpu->HL.hi);
    return 8;
}
int set_1_l(CPU *cpu){
    set(cpu, 1,&cpu->HL.lo);
    return 8;
}

int set_1_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    set(cpu,1, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int set_2_a(CPU *cpu){
    set(cpu, 2, &cpu->AF.hi);
    return 8;
}

int set_2_b(CPU *cpu){
    set(cpu, 2, &cpu->BC.hi);
    return 8;
}
int set_2_c(CPU *cpu){
    set(cpu, 2, &cpu->BC.lo);
    return 8;
}
int set_2_d(CPU *cpu){
    set(cpu, 2, &cpu->DE.hi);
    return 8;
}
int set_2_e(CPU *cpu){
    set(cpu, 2, &cpu->DE.lo);
    return 8;
}
int set_2_h(CPU *cpu){
    set(cpu,2, &cpu->HL.hi);
    return 8;
}
int set_2_l(CPU *cpu){
    set(cpu, 2,&cpu->HL.lo);
    return 8;
}

int set_2_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    set(cpu,2, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int set_3_a(CPU *cpu){
    set(cpu, 3, &cpu->AF.hi);
    return 8;
}

int set_3_b(CPU *cpu){
    set(cpu, 3, &cpu->BC.hi);
    return 8;
}
int set_3_c(CPU *cpu){
    set(cpu, 3, &cpu->BC.lo);
    return 8;
}
int set_3_d(CPU *cpu){
    set(cpu, 3, &cpu->DE.hi);
    return 8;
}
int set_3_e(CPU *cpu){
    set(cpu, 3, &cpu->DE.lo);
    return 8;
}
int set_3_h(CPU *cpu){
    set(cpu,3, &cpu->HL.hi);
    return 8;
}
int set_3_l(CPU *cpu){
    set(cpu, 3,&cpu->HL.lo);
    return 8;
}

int set_3_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    set(cpu,3, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int set_4_a(CPU *cpu){
    set(cpu, 4, &cpu->AF.hi);
    return 8;
}

int set_4_b(CPU *cpu){
    set(cpu, 4, &cpu->BC.hi);
    return 8;
}
int set_4_c(CPU *cpu){
    set(cpu, 4, &cpu->BC.lo);
    return 8;
}
int set_4_d(CPU *cpu){
    set(cpu, 4, &cpu->DE.hi);
    return 8;
}
int set_4_e(CPU *cpu){
    set(cpu, 4, &cpu->DE.lo);
    return 8;
}
int set_4_h(CPU *cpu){
    set(cpu,4, &cpu->HL.hi);
    return 8;
}
int set_4_l(CPU *cpu){
    set(cpu, 4,&cpu->HL.lo);
    return 8;
}

int set_4_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    set(cpu,4, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int set_5_a(CPU *cpu){
    set(cpu, 5, &cpu->AF.hi);
    return 8;
}

int set_5_b(CPU *cpu){
    set(cpu, 5, &cpu->BC.hi);
    return 8;
}
int set_5_c(CPU *cpu){
    set(cpu, 5, &cpu->BC.lo);
    return 8;
}
int set_5_d(CPU *cpu){
    set(cpu, 5, &cpu->DE.hi);
    return 8;
}
int set_5_e(CPU *cpu){
    set(cpu, 5, &cpu->DE.lo);
    return 8;
}
int set_5_h(CPU *cpu){
    set(cpu,5, &cpu->HL.hi);
    return 8;
}
int set_5_l(CPU *cpu){
    set(cpu, 5,&cpu->HL.lo);
    return 8;
}

int set_5_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    set(cpu,5, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int set_6_a(CPU *cpu){
    set(cpu, 6, &cpu->AF.hi);
    return 8;
}

int set_6_b(CPU *cpu){
    set(cpu, 6, &cpu->BC.hi);
    return 8;
}
int set_6_c(CPU *cpu){
    set(cpu, 6, &cpu->BC.lo);
    return 8;
}
int set_6_d(CPU *cpu){
    set(cpu, 6, &cpu->DE.hi);
    return 8;
}
int set_6_e(CPU *cpu){
    set(cpu, 6, &cpu->DE.lo);
    return 8;
}
int set_6_h(CPU *cpu){
    set(cpu,6, &cpu->HL.hi);
    return 8;
}
int set_6_l(CPU *cpu){
    set(cpu, 6,&cpu->HL.lo);
    return 8;
}

int set_6_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    set(cpu,6, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int set_7_a(CPU *cpu){
    set(cpu, 7, &cpu->AF.hi);
    return 8;
}

int set_7_b(CPU *cpu){
    set(cpu, 7, &cpu->BC.hi);
    return 8;
}
int set_7_c(CPU *cpu){
    set(cpu, 7, &cpu->BC.lo);
    return 8;
}
int set_7_d(CPU *cpu){
    set(cpu, 7, &cpu->DE.hi);
    return 8;
}
int set_7_e(CPU *cpu){
    set(cpu, 7, &cpu->DE.lo);
    return 8;
}
int set_7_h(CPU *cpu){
    set(cpu,7, &cpu->HL.hi);
    return 8;
}
int set_7_l(CPU *cpu){
    set(cpu, 7,&cpu->HL.lo);
    return 8;
}

int set_7_hlp(CPU *cpu){
    BYTE val = read_byte(cpu->mmu, cpu->HL.val);
    set(cpu,7, &val);
    write_byte(cpu->mmu, cpu->HL.val, val);
    return 16;
}

int jp_nz_nn(CPU *cpu) {
    SHORT address = ((read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val)));
    cpu->PC.val += 2;
    if(is_Z(cpu->AF)) return 12;
    else {
        cpu->PC.val = address;
        return 16;
    }
}

int jp_z_nn(CPU *cpu) {
    SHORT address = ((read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val)));
    cpu->PC.val += 2;
    if(!is_Z(cpu->AF)) return 12;
    else {
        cpu->PC.val = address;
        return 16;
    }
}

int jp_nc_nn(CPU *cpu) {
    SHORT address = ((read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val)));
    cpu->PC.val += 2;
    if(is_C(cpu->AF)) return 12;
    else {
        cpu->PC.val = address;
        return 16;
    }
}

int jp_c_nn(CPU *cpu) {
    SHORT address = ((read_byte(cpu->mmu, cpu->PC.val+1) << 8 | read_byte(cpu->mmu, cpu->PC.val)));
    cpu->PC.val += 2;
    if(!is_C(cpu->AF)) return 12;
    else {
        cpu->PC.val = address;
        return 16;
    }
}

int jp_hl(CPU *cpu){
    cpu->PC.val = cpu->HL.val;
    return 4;
}

void rst(CPU *cpu, BYTE value){
    _push(cpu, cpu->PC);
    cpu->PC.val = value;
}

int rst_0(CPU *cpu){
    rst(cpu, 0x0);
    return 32;
}

int rst_08(CPU *cpu){
    rst(cpu, 0x08);
    return 32;
}

int rst_10(CPU *cpu){
    rst(cpu, 0x10);
    return 32;
}

int rst_18(CPU *cpu){
    rst(cpu, 0x18);
    return 32;
}

int rst_20(CPU *cpu){
    rst(cpu, 0x20);
    return 32;
}

int rst_28(CPU *cpu){
    rst(cpu, 0x28);
    return 32;
}

int rst_30(CPU *cpu){
    rst(cpu, 0x30);
    return 32;
}

int rst_38(CPU *cpu){
    rst(cpu, 0x38);
    return 32;
}

int jp_nn(CPU *cpu) {
    SHORT address = ((read_byte(cpu->mmu, cpu->PC.val+1) << 8 ));
    address |= read_byte(cpu->mmu, cpu->PC.val);
    cpu->PC.val +=2;
    cpu->PC.val = address;
    return 16;
}

int cb_n(CPU *cpu){
    return execute_extended_opcode(cpu);
}

struct instruction instructions[] = {{ "NOP", NOP },                           // 0x00
                                     { "LD BC, 0x%04X",load_bc_nn },            // 0x01
                                     { "LD (BC), A", load_bc_a },               // 0x02
                                     { "INC BC", inc_bc},                     // 0x03
                                     { "INC B", inc_b },                       // 0x04
                                     { "DEC B", dec_b },                       // 0x05
                                     { "LD B, 0x%02X", load_b_n },               // 0x06
                                     { "RLCA", rlca },                         // 0x07
                                     { "LD (0x%04X), SP", load_nn_sp },         // 0x08
                                     { "ADD HL, BC", add_hl_bc },              // 0x09
                                     { "LD A, (BC)", load_a_bc },               // 0x0a
                                     { "DEC BC", dec_bc },                     // 0x0b
                                     { "INC C", inc_c },                       // 0x0c
                                     { "DEC C", dec_c },                       // 0x0d
                                     { "LD C, 0x%02X", load_c_n },               // 0x0e
                                     { "RRCA", rrca },                         // 0x0f
                                     { "STOP", stop },                         // 0x10
                                     { "LD DE, 0x%04X", load_de_nn },            // 0x11
                                     { "LD (DE), A", load_de_a },               // 0x12
                                     { "INC DE", inc_de },                     // 0x13
                                     { "INC D", inc_d },                       // 0x14
                                     { "DEC D", dec_d },                       // 0x15
                                     { "LD D, 0x%02X", load_d_n },               // 0x16
                                     { "RLA", rla },                           // 0x17
                                     { "JR 0x%02X", jr_n },                    // 0x18
                                     { "ADD HL, DE", add_hl_de },              // 0x19
                                     { "LD A, (DE)", load_a_de },               // 0x1a
                                     { "DEC DE", dec_de },                     // 0x1b
                                     { "INC E", inc_e },                       // 0x1c
                                     { "DEC E", dec_e },                       // 0x1d
                                     { "LD E, 0x%02X", load_e_n },               // 0x1e
                                     { "RRA", rra },                           // 0x1f
                                     { "JR NZ, 0x%02X", jr_nz_n },             // 0x20
                                     { "LD HL, 0x%04X", load_hl_nn },            // 0x21
                                     { "LDI (HL), A", loadi_hl_a },             // 0x22
                                     { "INC HL", inc_hl },                     // 0x23
                                     { "INC H", inc_h },                       // 0x24
                                     { "DEC H", dec_h },                       // 0x25
                                     { "LD H, 0x%02X", load_h_n },               // 0x26
                                     { "DAA", daa },                           // 0x27
                                     { "JR Z, 0x%02X", jr_z_n },               // 0x28
                                     { "ADD HL, HL", add_hl_hl },              // 0x29
                                     { "LDI A, (HL)", loadi_a_hl },             // 0x2a
                                     { "DEC HL", dec_hl },                     // 0x2b
                                     { "INC L", inc_l },                       // 0x2c
                                     { "DEC L", dec_l },                       // 0x2d
                                     { "LD L, 0x%02X", load_l_n },               // 0x2e
                                     { "CPL", cpl },                           // 0x2f
                                     { "JR NC, 0x%02X", jr_nc_n },             // 0x30
                                     { "LD SP, 0x%04X", load_sp_nn },            // 0x31
                                     { "LDD (HL), A", loadd_hl_a },             // 0x32
                                     { "INC SP", inc_sp },                     // 0x33
                                     { "INC (HL)", inc_hlp },                  // 0x34
                                     { "DEC (HL)", dec_hlp },                  // 0x35
                                     { "LD (HL), 0x%02X",load_hl_n },          // 0x36
                                     { "SCF", scf },                           // 0x37
                                     { "JR C, 0x%02X", jr_c_n },               // 0x38
                                     { "ADD HL, SP", add_hl_sp },              // 0x39
                                     { "LDD A, (HL)", loadd_a_hl },             // 0x3a
                                     { "DEC SP", dec_sp },                     // 0x3b
                                     { "INC A", inc_a },                       // 0x3c
                                     { "DEC A", dec_a },                       // 0x3d
                                     { "LD A, 0x%02X", load_a_n },               // 0x3e
                                     { "CCF", ccf },                           // 0x3f
                                     { "LD B, B", load_b_b },                       // 0x40
                                     { "LD B, C", load_b_c },                    // 0x41
                                     { "LD B, D", load_b_d },                    // 0x42
                                     { "LD B, E", load_b_e },                    // 0x43
                                     { "LD B, H", load_b_h },                    // 0x44
                                     { "LD B, L", load_b_l },                    // 0x45
                                     { "LD B, (HL)", load_b_hl },               // 0x46
                                     { "LD B, A", load_b_a },                    // 0x47
                                     { "LD C, B", load_c_b },                    // 0x48
                                     { "LD C, C", load_c_c },                       // 0x49
                                     { "LD C, D", load_c_d },                    // 0x4a
                                     { "LD C, E", load_c_e },                    // 0x4b
                                     { "LD C, H", load_c_h },                    // 0x4c
                                     { "LD C, L", load_c_l },                    // 0x4d
                                     { "LD C, (HL)", load_c_hl },               // 0x4e
                                     { "LD C, A", load_c_a },                    // 0x4f
                                     { "LD D, B", load_d_b },                    // 0x50
                                     { "LD D, C", load_d_c },                    // 0x51
                                     { "LD D, D", NOP},                       // 0x52
                                     { "LD D, E", load_d_e },                    // 0x53
                                     { "LD D, H", load_d_h },                    // 0x54
                                     { "LD D, L", load_d_l },                    // 0x55
                                     { "LD D, (HL)", load_d_hl },               // 0x56
                                     { "LD D, A", load_d_a },                    // 0x57
                                     { "LD E, B", load_e_b },                    // 0x58
                                     { "LD E, C", load_e_c },                    // 0x59
                                     { "LD E, D", load_e_d },                    // 0x5a
                                     { "LD E, E", NOP },                       // 0x5b
                                     { "LD E, H", load_e_h },                    // 0x5c
                                     { "LD E, L", load_e_l },                    // 0x5d
                                     { "LD E, (HL)", load_e_hl },               // 0x5e
                                     { "LD E, A", load_e_a },                    // 0x5f
                                     { "LD H, B", load_h_b },                    // 0x60
                                     { "LD H, C", load_h_c },                    // 0x61
                                     { "LD H, D", load_h_d },                    // 0x62
                                     { "LD H, E", load_h_e },                    // 0x63
                                     { "LD H, H", NOP },                       // 0x64
                                     { "LD H, L", load_h_l },                    // 0x65
                                     { "LD H, (HL)", load_h_hl },               // 0x66
                                     { "LD H, A", load_h_a },                    // 0x67
                                     { "LD L, B", load_l_b },                    // 0x68
                                     { "LD L, C", load_l_c },                    // 0x69
                                     { "LD L, D", load_l_d },                    // 0x6a
                                     { "LD L, E", load_l_e },                    // 0x6b
                                     { "LD L, H", load_l_h },                    // 0x6c
                                     { "LD L, L",   NOP },                       // 0x6d
                                     { "LD L, (HL)",   load_l_hl },               // 0x6e
                                     { "LD L, A",   load_l_a },                    // 0x6f
                                     { "LD (HL), B",   load_hl_b },               // 0x70
                                     { "LD (HL), C",   load_hl_c },               // 0x71
                                     { "LD (HL), D",   load_hl_d },               // 0x72
                                     { "LD (HL), E",   load_hl_e },               // 0x73
                                     { "LD (HL), H",   load_hl_h },               // 0x74
                                     { "LD (HL), L",   load_hl_l },               // 0x75
                                     { "HALT",   halt },                         // 0x76
                                     { "LD (HL), A",   load_hl_a },               // 0x77
                                     { "LD A, B",   load_a_b },                    // 0x78
                                     { "LD A, C",   load_a_c },                    // 0x79
                                     { "LD A, D",   load_a_d },                    // 0x7a
                                     { "LD A, E",   load_a_e },                    // 0x7b
                                     { "LD A, H",   load_a_h },                    // 0x7c
                                     { "LD A, L",   load_a_l},                    // 0x7d
                                     { "LD A, (HL)",   load_a_hl },               // 0x7e
                                     { "LD A, A",   NOP },                       // 0x7f
                                     { "ADD A, B",   add_a_b },                  // 0x80
                                     { "ADD A, C",   add_a_c },                  // 0x81
                                     { "ADD A, D",   add_a_d },                  // 0x82
                                     { "ADD A, E",   add_a_e },                  // 0x83
                                     { "ADD A, H",   add_a_h },                  // 0x84
                                     { "ADD A, L",   add_a_l },                  // 0x85
                                     { "ADD A, (HL)",   add_a_hl },             // 0x86
                                     { "ADD A",   add_a_a },                     // 0x87
                                     { "ADC B",   adc_a_b},                       // 0x88
                                     { "ADC C",   adc_a_c},                       // 0x89
                                     { "ADC D",   adc_a_d},                       // 0x8a
                                     { "ADC E",   adc_a_e},                       // 0x8b
                                     { "ADC H",   adc_a_h},                       // 0x8c
                                     { "ADC L",   adc_a_l},                       // 0x8d
                                     { "ADC (HL)",   adc_a_hl },                  // 0x8e
                                     { "ADC A",   adc_a_a },                       // 0x8f
                                     { "SUB B",   sub_b },                       // 0x90
                                     { "SUB C",   sub_c },                       // 0x91
                                     { "SUB D", sub_d },                       // 0x92
                                     { "SUB E", sub_e },                       // 0x93
                                     { "SUB H", sub_h },                       // 0x94
                                     { "SUB L", sub_l },                       // 0x95
                                     { "SUB (HL)", sub_hl},                  // 0x96
                                     { "SUB A", sub_a },                       // 0x97
                                     { "SBC B", subc_b },                       // 0x98
                                     { "SBC C", subc_c },                       // 0x99
                                     { "SBC D", subc_d },                       // 0x9a
                                     { "SBC E", subc_e },                       // 0x9b
                                     { "SBC H", subc_h },                       // 0x9c
                                     { "SBC L", subc_l },                       // 0x9d
                                     { "SBC (HL)", subc_hl},                  // 0x9e
                                     { "SBC A", subc_a },                       // 0x9f
                                     { "AND B", and_b },                       // 0xa0
                                     { "AND C", and_c },                       // 0xa1
                                     { "AND D", and_d },                       // 0xa2
                                     { "AND E", and_e },                       // 0xa3
                                     { "AND H", and_h },                       // 0xa4
                                     { "AND L", and_l },                       // 0xa5
                                     { "AND (HL)", and_hl},                  // 0xa6
                                     { "AND A",and_a},                       // 0xa7
                                     { "XOR B",   xor_b },                       // 0xa8
                                     { "XOR C",   xor_c },                       // 0xa9
                                     { "XOR D",   xor_d },                       // 0xaa
                                     { "XOR E",   xor_e },                       // 0xab
                                     { "XOR H",   xor_h },                       // 0xac
                                     { "XOR L",   xor_l },                       // 0xad
                                     { "XOR (HL)",   xor_hl },                  // 0xae
                                     { "XOR A",   xor_a },                       // 0xaf
                                     { "OR B",   or_b },                         // 0xb0
                                     { "OR C",   or_c },                         // 0xb1
                                     { "OR D",   or_d },                         // 0xb2
                                     { "OR E",   or_e },                         // 0xb3
                                     { "OR H",   or_h },                         // 0xb4
                                     { "OR L",   or_l },                         // 0xb5
                                     { "OR (HL)", or_hl },                    // 0xb6
                                     { "OR A",   or_a },                         // 0xb7
                                     { "CP B",   cp_b },                         // 0xb8
                                     { "CP C",   cp_c },                         // 0xb9
                                     { "CP D",   cp_d },                         // 0xba
                                     { "CP E",   cp_e },                         // 0xbb
                                     { "CP H",   cp_h },                         // 0xbc
                                     { "CP L",   cp_l },                         // 0xbd
                                     { "CP (HL)",   cp_hl },                    // 0xbe
                                     { "CP A",   cp_a },                         // 0xbf
                                     { "RET NZ",   ret_nz },                     // 0xc0
                                     { "POP BC",   pop_bc },                     // 0xc1
                                     { "JP NZ, 0x%04X", jp_nz_nn },            // 0xc2
                                     { "JP 0x%04X", jp_nn },                   // 0xc3
                                     { "CALL NZ, 0x%04X", call_nz_nn },        // 0xc4
                                     { "PUSH BC",   push_bc },                   // 0xc5
                                     { "ADD A, 0x%02X", add_a_n },             // 0xc6
                                     { "RST 0x00",   rst_0 },                    // 0xc7
                                     { "RET Z",   ret_z },                       // 0xc8
                                     { "RET",   ret },                           // 0xc9
                                     { "JP Z, 0x%04X", jp_z_nn },              // 0xca
                                     { "CB %02X",cb_n },                      // 0xcb
                                     { "CALL Z, 0x%04X", call_z_nn },          // 0xcc
                                     { "CALL 0x%04X", call_nn },               // 0xcd
                                     { "ADC 0x%02X", adc_a_n },                  // 0xce
                                     { "RST 0x08",   rst_08 },                   // 0xcf
                                     { "RET NC",   ret_nc },                     // 0xd0
                                     { "POP DE",   pop_de },                     // 0xd1
                                     { "JP NC, 0x%04X", jp_nc_nn },            // 0xd2
                                     { "UNKNOWN",   NULL },                 // 0xd3
                                     { "CALL NC, 0x%04X", call_nc_nn },        // 0xd4
                                     { "PUSH DE",   push_de },                   // 0xd5
                                     { "SUB 0x%02X", sub_nn },                  // 0xd6
                                     { "RST 0x10",   rst_10 },                   // 0xd7
                                     { "RET C",   ret_c },                       // 0xd8
                                     { "RETI",   reti },          // 0xd9
                                     { "JP C, 0x%04X", jp_c_nn },              // 0xda
                                     { "UNKNOWN",   NULL },                 // 0xdb
                                     { "CALL C, 0x%04X", call_c_nn },          // 0xdc
                                     { "UNKNOWN",   NULL },                 // 0xdd
                                     { "SBC 0x%02X", subc_nn },                  // 0xde
                                     { "RST 0x18",   rst_18 },                   // 0xdf
                                     { "LD (0xFF00 + 0x%02X), A", load_n_a },// 0xe0
                                     { "POP HL",   pop_hl },                     // 0xe1
                                     { "LD (0xFF00 + C), A",   load_cp_a },      // 0xe2
                                     { "UNKNOWN",   NULL },                 // 0xe3
                                     { "UNKNOWN",   NULL },                 // 0xe4
                                     { "PUSH HL",   push_hl },                   // 0xe5
                                     { "AND 0x%02X", and_nn },                  // 0xe6
                                     { "RST 0x20",   rst_20 },                   // 0xe7
                                     { "ADD SP,0x%02X", add_sp_n },            // 0xe8
                                     { "JP HL",   jp_hl },                       // 0xe9
                                     { "LD (0x%04X), A", load_nn_a },           // 0xea
                                     { "UNKNOWN",   NULL },                 // 0xeb
                                     { "UNKNOWN",   NULL },                 // 0xec
                                     { "UNKNOWN",   NULL },                 // 0xed
                                     { "XOR 0x%02X", xor_nn },                  // 0xee
                                     { "RST 0x28",   rst_28 },                   // 0xef
                                     { "LD A, (0xFF00 + 0x%02X)", load_a_np },// 0xf0
                                     { "POP AF",   pop_af },                     // 0xf1
                                     { "LD A, (0xFF00 + C)", load_a_cp},      // 0xf2
                                     { "DI",   di},                        // 0xf3
                                     { "UNKNOWN",   NULL },                 // 0xf4
                                     { "PUSH AF",  push_af },                   // 0xf5
                                     { "OR 0x%02X", or_nn },                    // 0xf6
                                     { "RST 0x30",   rst_30 },                   // 0xf7
                                     { "LD HL, SP+0x%02X",  },       // 0xf8
                                     { "LD SP, HL",   load_sp_hl },                // 0xf9
                                     { "LD A, (0x%04X)", load_a_nn },           // 0xfa
                                     { "EI",   ei },                             // 0xfb
                                     { "UNKNOWN", NULL },                 // 0xfc
                                     { "UNKNOWN", NULL },                 // 0xfd
                                     { "CP 0x%02X", cp_nn},                    // 0xfe
                                     { "RST 0x38", rst_38 },                   // 0xff
};

struct instruction extendedInstructions[] = {
        { "RLC B", rlc_b },           // 0x00
        { "RLC C", rlc_c },           // 0x01
        { "RLC D", rlc_d },           // 0x02
        { "RLC E", rlc_e },           // 0x03
        { "RLC H", rlc_h },           // 0x04
        { "RLC L", rlc_l },           // 0x05
        { "RLC (HL)", rlc_hlp },      // 0x06
        { "RLC A", rlc_a },           // 0x07
        { "RRC B", rrc_b },           // 0x08
        { "RRC C", rrc_c },           // 0x09
        { "RRC D", rrc_d },           // 0x0a
        { "RRC E", rrc_e },           // 0x0b
        { "RRC H", rrc_h },           // 0x0c
        { "RRC L", rrc_l },           // 0x0d
        { "RRC (HL)", rrc_hlp },      // 0x0e
        { "RRC A", rrc_a },           // 0x0f
        { "RL B", rl_b },             // 0x10
        { "RL C", rl_c },             // 0x11
        { "RL D", rl_d },             // 0x12
        { "RL E", rl_e },             // 0x13
        { "RL H", rl_h },             // 0x14
        { "RL L", rl_l },             // 0x15
        { "RL (HL)", rl_hlp },        // 0x16
        { "RL A", rl_a },             // 0x17
        { "RR B", rr_b },             // 0x18
        { "RR C", rr_c },             // 0x19
        { "RR D", rr_d },             // 0x1a
        { "RR E", rr_e },             // 0x1b
        { "RR H", rr_h },             // 0x1c
        { "RR L", rr_l },             // 0x1d
        { "RR (HL)", rr_hlp },        // 0x1e
        { "RR A", rr_a },             // 0x1f
        { "SLA B", sla_b },           // 0x20
        { "SLA C", sla_c },           // 0x21
        { "SLA D", sla_d },           // 0x22
        { "SLA E", sla_e },           // 0x23
        { "SLA H", sla_h },           // 0x24
        { "SLA L", sla_l },           // 0x25
        { "SLA (HL)", sla_hlp },      // 0x26
        { "SLA A", sla_a },           // 0x27
        { "SRA B", sra_b },           // 0x28
        { "SRA C", sra_c },           // 0x29
        { "SRA D", sra_d },           // 0x2a
        { "SRA E", sra_e },           // 0x2b
        { "SRA H", sra_h },           // 0x2c
        { "SRA L", sra_l },           // 0x2d
        { "SRA (HL)", sra_hlp },      // 0x2e
        { "SRA A", sra_a },           // 0x2f
        { "SWAP B", swap_b },         // 0x30
        { "SWAP C", swap_c },         // 0x31
        { "SWAP D", swap_d },         // 0x32
        { "SWAP E", swap_e },         // 0x33
        { "SWAP H", swap_h },         // 0x34
        { "SWAP L", swap_l },         // 0x35
        { "SWAP (HL)", swap_hlp },    // 0x36
        { "SWAP A", swap_a },         // 0x37
        { "SRL B", srl_b },           // 0x38
        { "SRL C", srl_c },           // 0x39
        { "SRL D", srl_d },           // 0x3a
        { "SRL E", srl_e },           // 0x3b
        { "SRL H", srl_h },           // 0x3c
        { "SRL L", srl_l },           // 0x3d
        { "SRL (HL)", srl_hlp },      // 0x3e
        { "SRL A", srl_a },           // 0x3f
        { "BIT 0, B", bit_0_b },      // 0x40
        { "BIT 0, C", bit_0_c },      // 0x41
        { "BIT 0, D", bit_0_d },      // 0x42
        { "BIT 0, E", bit_0_e },      // 0x43
        { "BIT 0, H", bit_0_h },      // 0x44
        { "BIT 0, L", bit_0_l },      // 0x45
        { "BIT 0, (HL)", bit_0_hlp }, // 0x46
        { "BIT 0, A", bit_0_a },      // 0x47
        { "BIT 1, B", bit_1_b },      // 0x48
        { "BIT 1, C", bit_1_c },      // 0x49
        { "BIT 1, D", bit_1_d },      // 0x4a
        { "BIT 1, E", bit_1_e },      // 0x4b
        { "BIT 1, H", bit_1_h },      // 0x4c
        { "BIT 1, L", bit_1_l },      // 0x4d
        { "BIT 1, (HL)", bit_1_hlp }, // 0x4e
        { "BIT 1, A", bit_1_a },      // 0x4f
        { "BIT 2, B", bit_2_b },      // 0x50
        { "BIT 2, C", bit_2_c },      // 0x51
        { "BIT 2, D", bit_2_d },      // 0x52
        { "BIT 2, E", bit_2_e },      // 0x53
        { "BIT 2, H", bit_2_h },      // 0x54
        { "BIT 2, L", bit_2_l },      // 0x55
        { "BIT 2, (HL)", bit_2_hlp }, // 0x56
        { "BIT 2, A", bit_2_a },      // 0x57
        { "BIT 3, B", bit_3_b },      // 0x58
        { "BIT 3, C", bit_3_c },      // 0x59
        { "BIT 3, D", bit_3_d },      // 0x5a
        { "BIT 3, E", bit_3_e },      // 0x5b
        { "BIT 3, H", bit_3_h },      // 0x5c
        { "BIT 3, L", bit_3_l },      // 0x5d
        { "BIT 3, (HL)", bit_3_hlp }, // 0x5e
        { "BIT 3, A", bit_3_a },      // 0x5f
        { "BIT 4, B", bit_4_b },      // 0x60
        { "BIT 4, C", bit_4_c },      // 0x61
        { "BIT 4, D", bit_4_d },      // 0x62
        { "BIT 4, E", bit_4_e },      // 0x63
        { "BIT 4, H", bit_4_h },      // 0x64
        { "BIT 4, L", bit_4_l },      // 0x65
        { "BIT 4, (HL)", bit_4_hlp }, // 0x66
        { "BIT 4, A", bit_4_a },      // 0x67
        { "BIT 5, B", bit_5_b },      // 0x68
        { "BIT 5, C", bit_5_c },      // 0x69
        { "BIT 5, D", bit_5_d },      // 0x6a
        { "BIT 5, E", bit_5_e },      // 0x6b
        { "BIT 6, H", bit_5_h },      // 0x6c
        { "BIT 6, L", bit_5_l },      // 0x6d
        { "BIT 5, (HL)", bit_5_hlp }, // 0x6e
        { "BIT 5, A", bit_5_a },      // 0x6f
        { "BIT 6, B", bit_6_b },      // 0x70
        { "BIT 6, C", bit_6_c },      // 0x71
        { "BIT 6, D", bit_6_d },      // 0x72
        { "BIT 6, E", bit_6_e },      // 0x73
        { "BIT 6, H", bit_6_h },      // 0x74
        { "BIT 6, L", bit_6_l },      // 0x75
        { "BIT 6, (HL)", bit_6_hlp }, // 0x76
        { "BIT 6, A", bit_6_a },      // 0x77
        { "BIT 7, B", bit_7_b },      // 0x78
        { "BIT 7, C", bit_7_c },      // 0x79
        { "BIT 7, D", bit_7_d },      // 0x7a
        { "BIT 7, E", bit_7_e },      // 0x7b
        { "BIT 7, H", bit_7_h },      // 0x7c
        { "BIT 7, L", bit_7_l },      // 0x7d
        { "BIT 7, (HL)", bit_7_hlp }, // 0x7e
        { "BIT 7, A", bit_7_a },      // 0x7f
        { "RES 0, B", res_0_b },      // 0x80
        { "RES 0, C", res_0_c },      // 0x81
        { "RES 0, D", res_0_d },      // 0x82
        { "RES 0, E", res_0_e },      // 0x83
        { "RES 0, H", res_0_h },      // 0x84
        { "RES 0, L", res_0_l },      // 0x85
        { "RES 0, (HL)", res_0_hlp }, // 0x86
        { "RES 0, A", res_0_a },      // 0x87
        { "RES 1, B", res_1_b },      // 0x88
        { "RES 1, C", res_1_c },      // 0x89
        { "RES 1, D", res_1_d },      // 0x8a
        { "RES 1, E", res_1_e },      // 0x8b
        { "RES 1, H", res_1_h },      // 0x8c
        { "RES 1, L", res_1_l },      // 0x8d
        { "RES 1, (HL)", res_1_hlp }, // 0x8e
        { "RES 1, A", res_1_a },      // 0x8f
        { "RES 2, B", res_2_b },      // 0x90
        { "RES 2, C", res_2_c },      // 0x91
        { "RES 2, D", res_2_d },      // 0x92
        { "RES 2, E", res_2_e },      // 0x93
        { "RES 2, H", res_2_h },      // 0x94
        { "RES 2, L", res_2_l },      // 0x95
        { "RES 2, (HL)", res_2_hlp }, // 0x96
        { "RES 2, A", res_2_a },      // 0x97
        { "RES 3, B", res_3_b },      // 0x98
        { "RES 3, C", res_3_c },      // 0x99
        { "RES 3, D", res_3_d },      // 0x9a
        { "RES 3, E", res_3_e },      // 0x9b
        { "RES 3, H", res_3_h },      // 0x9c
        { "RES 3, L", res_3_l },      // 0x9d
        { "RES 3, (HL)", res_3_hlp }, // 0x9e
        { "RES 3, A", res_3_a },      // 0x9f
        { "RES 4, B", res_4_b },      // 0xa0
        { "RES 4, C", res_4_c },      // 0xa1
        { "RES 4, D", res_4_d },      // 0xa2
        { "RES 4, E", res_4_e },      // 0xa3
        { "RES 4, H", res_4_h },      // 0xa4
        { "RES 4, L", res_4_l },      // 0xa5
        { "RES 4, (HL)", res_4_hlp }, // 0xa6
        { "RES 4, A", res_4_a },      // 0xa7
        { "RES 5, B", res_5_b },      // 0xa8
        { "RES 5, C", res_5_c },      // 0xa9
        { "RES 5, D", res_5_d },      // 0xaa
        { "RES 5, E", res_5_e },      // 0xab
        { "RES 5, H", res_5_h },      // 0xac
        { "RES 5, L", res_5_l },      // 0xad
        { "RES 5, (HL)", res_5_hlp }, // 0xae
        { "RES 5, A", res_5_a },      // 0xaf
        { "RES 6, B", res_6_b },      // 0xb0
        { "RES 6, C", res_6_c },      // 0xb1
        { "RES 6, D", res_6_d },      // 0xb2
        { "RES 6, E", res_6_e },      // 0xb3
        { "RES 6, H", res_6_h },      // 0xb4
        { "RES 6, L", res_6_l },      // 0xb5
        { "RES 6, (HL)", res_6_hlp }, // 0xb6
        { "RES 6, A", res_6_a },      // 0xb7
        { "RES 7, B", res_7_b },      // 0xb8
        { "RES 7, C", res_7_c },      // 0xb9
        { "RES 7, D", res_7_d },      // 0xba
        { "RES 7, E", res_7_e },      // 0xbb
        { "RES 7, H", res_7_h },      // 0xbc
        { "RES 7, L", res_7_l },      // 0xbd
        { "RES 7, (HL)", res_7_hlp }, // 0xbe
        { "RES 7, A", res_7_a },      // 0xbf
        { "SET 0, B", set_0_b },      // 0xc0
        { "SET 0, C", set_0_c },      // 0xc1
        { "SET 0, D", set_0_d },      // 0xc2
        { "SET 0, E", set_0_e },      // 0xc3
        { "SET 0, H", set_0_h },      // 0xc4
        { "SET 0, L", set_0_l },      // 0xc5
        { "SET 0, (HL)", set_0_hlp }, // 0xc6
        { "SET 0, A", set_0_a },      // 0xc7
        { "SET 1, B", set_1_b },      // 0xc8
        { "SET 1, C", set_1_c },      // 0xc9
        { "SET 1, D", set_1_d },      // 0xca
        { "SET 1, E", set_1_e },      // 0xcb
        { "SET 1, H", set_1_h },      // 0xcc
        { "SET 1, L", set_1_l },      // 0xcd
        { "SET 1, (HL)", set_1_hlp }, // 0xce
        { "SET 1, A", set_1_a },      // 0xcf
        { "SET 2, B", set_2_b },      // 0xd0
        { "SET 2, C", set_2_c },      // 0xd1
        { "SET 2, D", set_2_d },      // 0xd2
        { "SET 2, E", set_2_e },      // 0xd3
        { "SET 2, H", set_2_h },      // 0xd4
        { "SET 2, L", set_2_l },      // 0xd5
        { "SET 2, (HL)", set_2_hlp }, // 0xd6
        { "SET 2, A", set_2_a },      // 0xd7
        { "SET 3, B", set_3_b },      // 0xd8
        { "SET 3, C", set_3_c },      // 0xd9
        { "SET 3, D", set_3_d },      // 0xda
        { "SET 3, E", set_3_e },      // 0xdb
        { "SET 3, H", set_3_h },      // 0xdc
        { "SET 3, L", set_3_l },      // 0xdd
        { "SET 3, (HL)", set_3_hlp }, // 0xde
        { "SET 3, A", set_3_a },      // 0xdf
        { "SET 4, B", set_4_b },      // 0xe0
        { "SET 4, C", set_4_c },      // 0xe1
        { "SET 4, D", set_4_d },      // 0xe2
        { "SET 4, E", set_4_e },      // 0xe3
        { "SET 4, H", set_4_h },      // 0xe4
        { "SET 4, L", set_4_l },      // 0xe5
        { "SET 4, (HL)", set_4_hlp }, // 0xe6
        { "SET 4, A", set_4_a },      // 0xe7
        { "SET 5, B", set_5_b },      // 0xe8
        { "SET 5, C", set_5_c },      // 0xe9
        { "SET 5, D", set_5_d },      // 0xea
        { "SET 5, E", set_5_e },      // 0xeb
        { "SET 5, H", set_5_h },      // 0xec
        { "SET 5, L", set_5_l },      // 0xed
        { "SET 5, (HL)", set_5_hlp }, // 0xee
        { "SET 5, A", set_5_a },      // 0xef
        { "SET 6, B", set_6_b },      // 0xf0
        { "SET 6, C", set_6_c },      // 0xf1
        { "SET 6, D", set_6_d },      // 0xf2
        { "SET 6, E", set_6_e },      // 0xf3
        { "SET 6, H", set_6_h },      // 0xf4
        { "SET 6, L", set_6_l },      // 0xf5
        { "SET 6, (HL)", set_6_hlp }, // 0xf6
        { "SET 6, A", set_6_a },      // 0xf7
        { "SET 7, B", set_7_b },      // 0xf8
        { "SET 7, C", set_7_c },      // 0xf9
        { "SET 7, D", set_7_d },      // 0xfa
        { "SET 7, E", set_7_e },      // 0xfb
        { "SET 7, H", set_7_h },      // 0xfc
        { "SET 7, L", set_7_l },      // 0xfd
        { "SET 7, (HL)", set_7_hlp }, // 0xfe
        { "SET 7, A", set_7_a },      // 0xff
};

