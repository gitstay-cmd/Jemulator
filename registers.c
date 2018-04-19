#include <stdio.h>

#include "registers.h"

suc_t is_Z(register_u AF){
    return (suc_t)((AF.hi & ZERO)? 1: 0);
}

suc_t is_O(register_u AF){
    return (suc_t)((AF.hi & OPER)? 1: 0);
}

suc_t is_H(register_u AF){
	return (suc_t)((AF.hi & HALFC)? 1: 0);
}

suc_t is_C(register_u AF){
    return (suc_t)((AF.hi & CARRY)? 1: 0);
}

