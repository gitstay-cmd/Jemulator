#include "mmu.h"
#include "interrupts.h"
#include "joypad.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MMU *mmu_init(char *filename){
    
    MMU *mmu = (MMU *)malloc(sizeof(MMU));

	memset(mmu->_rom, 0, 0x10000);
	memset(mmu->_ram_banks, 0,0x8000);
	memset(mmu->_cartridge, 0, 0x200000);

	mmu->_rom[0xFF10] = 0x80;
	mmu->_rom[0xFF11] = 0xBF;
	mmu->_rom[0xFF12] = 0xF3;
	mmu->_rom[0xFF14] = 0xBF;
	mmu->_rom[0xFF16] = 0x3F;
	mmu->_rom[0xFF19] = 0xBF;
	mmu->_rom[0xFF1A] = 0x7F;
	mmu->_rom[0xFF1B] = 0xFF;
	mmu->_rom[0xFF1C] = 0x9F;
	mmu->_rom[0xFF1E] = 0xBF;
	mmu->_rom[0xFF20] = 0xFF;
	mmu->_rom[0xFF23] = 0xBF;
	mmu->_rom[0xFF24] = 0x77;
	mmu->_rom[0xFF25] = 0xF3;
	mmu->_rom[0xFF26] = 0xF1;
	mmu->_rom[0xFF40] = 0x91;
	mmu->_rom[0xFF47] = 0xFC;
	mmu->_rom[0xFF48] = 0xFF;
	mmu->_rom[0xFF49] = 0xFF;

	FILE *fp;
	fp = fopen(filename, "r");
	
	fread(mmu->_cartridge, 1, 0x200000, fp);

	memcpy(mmu->_rom, mmu->_cartridge, 0x8000);

	mmu->_rom_bank = 1;

	mmu->_mbc2 = 0;

	switch(read_byte(mmu, 0x147)){
    	case 0: mmu->_mbc1 = 0; break;
    	case 1:
       	case 2:
        case 3: mmu->_mbc1 = 1; break;
        case 5: mmu->_mbc1 = 1; break;
        case 6: mmu->_mbc1 = 1; break;
        default: break;
	}

	switch(read_byte(mmu, 0x149)){
    	case 0: mmu->_num_ram_banks = 0; break;
    	case 1:
    	case 2: mmu->_num_ram_banks = 1; break;
    	case 3: mmu->_num_ram_banks = 4; break;
    	case 4: mmu->_num_ram_banks = 16; break;
        default:
            break;
	}

	reset();

	mmu->_ram_bank = 0;

	mmu->_rom_bank = 1;

	mmu->_rom_banking = 1;

	mmu->_enable_ram = 0;

	mmu->lcd = (LCD*)malloc(sizeof(LCD));

	init_screen(mmu->lcd);

	mmu->timer = (TIMER*)malloc(sizeof(TIMER));

	timer_init(mmu->timer);

	mmu->joy = (joypad*)malloc(sizeof(joypad));
	joy_init(mmu->joy);
    return mmu;
}

void do_dma(MMU *mmu, BYTE value){
    SHORT address = value << 8;
    for(BYTE i = 0; i < 0xA0; i++){
        write_byte(mmu, (BYTE)0xFE00+i, read_byte(mmu, address+i));
    }
    
}

void change_rom_ram_mode(MMU *mmu, BYTE value){
    BYTE data = (BYTE)(value & 0x1);
    mmu->_rom_banking = (BYTE)((!data)? 1:0);
    if(mmu->_rom_banking) mmu->_ram_bank = 0;
}

void ram_bank_change(MMU *mmu, BYTE value){
    mmu->_ram_bank = (BYTE)(value & 0x3);
}

void change_hi_rom_bank(MMU *mmu, BYTE value){
    mmu->_rom_bank &= 0x1F;
    value &= 0xA4;
    mmu->_rom_bank |= value;
    if(!mmu->_rom_bank) mmu->_rom_bank++;
}

void change_lo_rom_bank(MMU *mmu, BYTE value){
    if(mmu->_mbc2){
        mmu->_rom_bank = (BYTE)(value & 0xF);
        if(!mmu->_rom_bank) mmu->_rom_bank++;
        return;
    }
    mmu->_rom_bank &= 224; // turn off the lower 5
    mmu->_rom_bank |= (BYTE)(value & 0x1F);
    if(!mmu->_rom_bank) mmu->_rom_bank++;
}

void enable_ram_bank(MMU *mmu, SHORT address, BYTE value){
    if(mmu->_mbc2){
        if(address & 0x10) return;
    }
    BYTE testData = (BYTE)(value & 0xF);
    if(testData == 0xA) {
        mmu->_enable_ram = 1;
    } else if (testData == 0x0)
    	mmu->_enable_ram = 0;
}


void timer_write(TIMER *timer, SHORT address, BYTE value) {
    BYTE currentfreq;
    switch(address){
        case 0xFF04:
            timer->d_r = 0;
            break;
        case 0xFF05:
            timer->tima = value;
            break;
        case 0xFF06:
            timer->tma = value;
            break;
        case TMC:
            currentfreq = GetClockFreq(timer);
            timer->tmc = value;
            BYTE newfreq = GetClockFreq(timer);

            if(currentfreq != newfreq){
                SetClockFreq(timer);
            }
            break;
        default:
            break;
    }
}

BYTE timer_read(TIMER *timer, SHORT address){
    switch(address){
        case 0xFF04:
            return timer->d_r;
        case 0xFF05:
            return timer->tima;
        case 0xFF06:
            return timer->tma;
        case 0xFF07:
            return timer->tmc;
        default:
            return 0;
            break;
    }
}

void lcd_write(LCD *lcd, SHORT address, BYTE value){
    switch(address){
        case 0xFF40:
            lcd->ctl_r = value;
            break;
        case 0xFF41:
            lcd->status = value;
            break;
        case 0xFF42:
            lcd->scrolly = value;
            break;
        case 0xFF43:
            lcd->scrollx = value;
            break;
        case 0xFF44:
            lcd->sc_l = 0;
            break;
        case 0xFF45:
            lcd->sl_g = value;
            break;
        case 0xFF4A:
            lcd->windowy = value;
            break;
        case 0xFF4B:
            lcd->windowx = value;
            break;
        default:
            break;
    }
}


BYTE lcd_read(LCD *lcd, SHORT address){
    switch(address){
        case 0xFF40:
            return lcd->ctl_r;
        case 0xFF41:
            return lcd->status;
        case 0xFF42:
            return lcd->scrolly;
        case 0xFF43:
            return lcd->scrollx;
        case 0xFF44:
            return lcd->sc_l;
        case 0xFF45:
            return lcd->sl_g;
        case 0xFF4A:
            return lcd->windowy;
        case 0xFF4B:
            return lcd->windowx;
        default:
            return 0;
            break;
    }
}

void rom_bank_handler(MMU *mmu, SHORT address, BYTE value){

    switch(address & 0xF000){
        case 0x0000:
        case 0x1000:
           	if(mmu->_mbc1 || mmu->_mbc2){
               	enable_ram_bank(mmu, address, value);
           	}
            break;
        case 0x2000:
        case 0x3000:
            if(mmu->_mbc1 || mmu->_mbc2){
                change_lo_rom_bank(mmu, value);
            }
            break;
       	case 0x4000:
        case 0x5000:
            if(mmu->_mbc1){
                if(mmu->_rom_banking){
                    change_hi_rom_bank(mmu, value);
                }else{
                    ram_bank_change(mmu, value);
                }
            }
            break;
        case 0x6000:
        case 0x7000:
            if(mmu->_mbc1){
                change_rom_ram_mode(mmu, value);
            }
            break;
        default:
                break;
   	}	
}

void _push(void *cpu, register_u X){
    ((CPU*)cpu)->SP.val-= 2;
    write_byte(((CPU*)cpu)->mmu, ((CPU*)cpu)->SP.val, X.lo);
    write_byte(((CPU*)cpu)->mmu, (SHORT)(((CPU*)cpu)->SP.val+1), X.hi);
}

SHORT _pop(void *cpu){
    SHORT valu = read_byte(((CPU*)cpu)->mmu, ((CPU*)cpu)->SP.val);
    valu |= read_byte(((CPU*)cpu)->mmu, ((CPU*)cpu)->SP.val+1) << 8;
    ((CPU*)cpu)->SP.val += 2;
    return valu;
}

suc_t write_byte(MMU *mmu, SHORT address, BYTE value){
   
    switch(address & 0xF000){
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            rom_bank_handler(mmu, address, value);
            break;
        case 0x8000:
        case 0x9000:
            mmu->_rom[address] = value;
            break;
        case 0xA000:
        case 0xB000:
            if(mmu->_enable_ram){
                address %= 0xA000;
                mmu->_rom[address + (mmu->_ram_bank *0x2000)] = value;
            }
            break;
        case 0xC000:
      	case 0xD000:
          	if(address <= 0xDDFF){
				mmu->_rom[0xE000 + (address - 0xC000)] = value;
				mmu->_rom[address] = value;
			}
			break;
        case 0xE000:
        case 0xF000:
            if(address <= 0xFDFF){
                mmu->_rom[0xC000 + (address - 0xE000)] = value;
                mmu->_rom[address] = value;
            }else if (address >= 0xFEA0 && address <= 0xFEFF){
                return 0;
            }else if(address == TMC || (address >= 0xFF04 && address <= 0xFF07)){
                timer_write(mmu->timer, address, value);
            }else if(address == 0xFF40 || address == 0xFF41 || address == 0xFF42 || address == 0xFF43 || address == 0xFF44 || address == 0xFF45 || address == 0xFF4A || address == 0xFF4B || address == 0xFF44){
            	lcd_write(mmu->lcd, address, value);
            }else if(address == 0xFF46){
                do_dma(mmu, value);
            }else if(address == 0xFF0F || address == 0xFFFF){
                interrupt_write(address, value);
            }else {
                mmu->_rom[address] = value;
            }
            break;
        default:
            mmu->_rom[address] = value;
            break;
    }
    return 1;
}

BYTE read_byte(MMU *mmu, SHORT address){
    switch(address & 0xF000){
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            address %= 0x4000;
            return mmu->_cartridge[address + (mmu->_rom_bank * 0x4000)];
       	case 0xA000:
        case 0xB000:
            address %= 0xA000;
            return mmu->_ram_banks[address + (mmu->_ram_bank * 0x2000)];
        default:
            if(address == 0xFF40 || address == 0xFF41 || address == 0xFF42 || address == 0xFF43 || address == 0xFF44 || address == 0xFF45 || address == 0xFF4A || address == 0xFF4B){
                return lcd_read(mmu->lcd, address);
            }else if((address >= 0xFF04 && address <= 0xFF07)){
                return timer_read(mmu->timer, address);
            }else if(address == 0xFF0F || address == 0xFFFF){
                return interrupt_read(address);
            }else if(address == 0xFF00){
                return GetJoyPadState(mmu);
            }
            return mmu->_rom[address];
    }
}


