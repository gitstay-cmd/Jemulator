#include "includes.h"
#include "interrupts.h"
#include "joypad.h"
#include "mmu.h"
#include <stdio.h>

void joy_init(joypad *joy){
    joy->status = 0xFF;

}

void KeyPressed(void *mmu, int key){
    BYTE previouslyUnset = 0;
    joypad *joy = ((MMU*)mmu)->joy;
    /*if(TestBit(_joypad_state, key)==false){
        previouslyUnset = 1;
    }*/
    if(!(joy->status & (1 <<key))){
        previouslyUnset = 1;
    }

    joy->status &= ~(1 << key);

    BYTE button = 1;

    if(key > 3){
        button = 1;
    }else{
        button = 0;
    }

    BYTE keyReq = ((MMU*)mmu)->_rom[0xFF00];
    BYTE requestInterrupt = 0;

    if(button && !(keyReq & (1 << 5))){
        requestInterrupt = 1;
    }

    else if(!button && !(keyReq & (1 <<4))){
        requestInterrupt = 1;
    }

    if(requestInterrupt && !previouslyUnset){
        RequestInterrupt(4);
    }
}

void KeyReleased(void *mmu, int key){
    joypad *joy = ((MMU*)mmu)->joy;
    joy->status |= (1<< key);
}

BYTE GetJoyPadState(void *mmu){
    joypad *joy = ((MMU*)mmu)->joy;
    BYTE res = ((MMU*)mmu)->_rom[0xFF00];
    res &= 0xF0;
    if(!(res & (1 <<5))){
        BYTE topJoyPad = (joy->status>> 4) & 0x0F;
        res |= topJoyPad;
        res |= 0xC0;
    }
    else if(!(res & (1 <<4))){

        BYTE bottomJoypad = (BYTE)(joy->status & 0xF);
        res |= bottomJoypad;
        res |= 0xC0;
    }else {
        res |= 0x0F;
    }
    return res;
}
