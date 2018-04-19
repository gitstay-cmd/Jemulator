#ifndef mmu_h_INCLUDED
#define mmu_h_INCLUDED

#include "includes.h"
#include "lcd.h"
#include "timer.h"
#include "joypad.h"
#include "registers.h"

typedef BYTE suc_t;

//0000-3FFF 16KB ROM BANK 00
//4000-7FFF 16KB BANK 1+
//8000-9FFF 8KB VRAM
//A000-BFFF External RAM BANK
//C000-CFFF 4KB WORK RAM BANK 0
//D000-DFFF 4KB WORK RAM BANK 1-7
//E000-FDFF ECHO OF C000-DDFF 
//FE00-FE9F Sprite Attribute Table (OAM)
//FEA0-FEFF Not Usable
//FF00-FF7F I/O Ports
//FF80-FFFE High RAM 
//FFFF 		Interrupt Enable Register

typedef struct mmu_S{
    BYTE _cartridge[0x200000];
    BYTE _rom[0x10000];
    BYTE _ram_banks[0x8000];
    BYTE _rom_bank;
    BYTE _rom_banking;
    BYTE _num_ram_banks;
    BYTE _ram_bank;
    BYTE _enable_ram;
    BYTE _mbc1;
    BYTE _mbc2;
    joypad *joy;
    TIMER *timer;
    LCD *lcd;
} MMU;

suc_t write_byte(MMU *mmu, SHORT address, BYTE value);
BYTE read_byte(MMU *mmu, SHORT address);
MMU *mmu_init(char *filename);
void _push(void *cpu, register_u X);
SHORT _pop(void *cpu);
#endif // mmu_h_INCLUDED

