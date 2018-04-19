#ifndef joypad_h_INCLUDED
#define joypad_h_INCLUDED

//Bit 7 - Not Used
//Bit 6 - Not Used
//Bit 5 - P15 Select Button Keys(0 = Select)
//Bit 4 - P14 Select Direction Keys(0 = Select)
//Bit 3 - P13 Input Down or Start(0=Pressed)(Read Only)
//Bit 2 - P12 Input Up or Select(0=Pressed)(Read Only)
//Bit 1 - P11 Input Left or Button B(0=Pressed)(Read Only)
//Bit 0 - P10 Input Right or Button A(0=Pressed)(Read Only)
#include "includes.h"
typedef struct _joy_pad{
	BYTE status;
}joypad;

BYTE GetJoyPadState(void *mmu);
void joy_init(joypad *joy);
void KeyPressed(void *mmu, int key);
void KeyReleased(void *mmu, int key);
#endif // joypad_h_INCLUDED

