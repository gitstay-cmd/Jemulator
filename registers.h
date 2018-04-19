#ifndef registers_h_INCLUDED
#define registers_h_INCLUDED
#include "includes.h"

typedef unsigned char suc_t;
// A : F
// B : C
// D : E
// H : L
// SP
// PC 
// F:
    // [ Zero | Operation | Half-carry | Carry | x | x | x | x ]

#define ZERO	0x80 
#define	OPER	0x40
#define HALFC	0x20
#define CARRY	0x10

typedef union {

    SHORT val;
    struct {
//        BYTE hi;
//        BYTE lo;
	   BYTE lo;
	   BYTE hi;
        
	};

}register_u;

//Function to read the Zero bit in the F register
suc_t is_Z(register_u AF);

//Function to read the Operation bit in the F register
suc_t is_O(register_u AF);

//Function to read the Half-Carry bit in the F register
suc_t is_H(register_u AF);

//Function to read the Carry bit in the F register
suc_t is_C(register_u AF);

#endif // registers_h_INCLUDED

