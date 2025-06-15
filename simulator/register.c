#include <stdint.h>
#include "register.h"


// Read from any register
int32_t read_register(reg_file* rf, reg_name reg) {
    if (reg < 0 || reg >= REG_NUM) {
        return 0;
    }
    return rf->reg_array[reg];
}

// Write only to registers 2 to 15
void write_register(reg_file* rf, reg_name reg, int32_t value) {
    if (reg <= REG_IMM || reg >= REG_NUM) {
		return; // Ignore writes to invalid registers
    }
    rf->reg_array[reg] = value;
}