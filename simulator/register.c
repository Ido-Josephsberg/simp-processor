#include <stdint.h>
#include "simulator.h"
#include "register.h"


// Read from any register
int32_t read_register(Simulator* sim, reg_name reg) {
    if (reg < 0 || reg >= REG_NUM) {
        return 0;
    }
    return sim->reg_array[reg];
}

// Write only to registers 2 to 15
void write_register(Simulator* sim, reg_name reg, int32_t value) {
    if (reg <= IMM || reg >= REG_NUM) {
		return; // Ignore writes to invalid registers
    }
    sim->reg_array[reg] = value;
}

// Write to IMM register 
void write_register_imm(Simulator* sim, int32_t value) {
    sim->reg_array[IMM] = value;
}