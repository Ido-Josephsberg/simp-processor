#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include <stdlib.h>

#define REG_NUM 16

typedef enum {
	REG_ZERO,	// 0
	REG_IMM,	// 1
	REG_V0,		// 2
    REG_A0,     // 3
    REG_A1,     // 4
    REG_A2,     // 5
	REG_A3,     // 6
    REG_T0,     // 7
    REG_T1,     // 8
    REG_T2,     // 9
    REG_S0,     // 10
    REG_S1,     // 11
    REG_S2,     // 12
    REG_GP,     // 13
    REG_SP,     // 14
    REG_RA      // 15
} reg_name;

// A struct with array to hold the registers' values
typedef struct {
    int32_t reg_array[REG_NUM];
} reg_file;

// Register Related Functions
int32_t read_register(reg_file* rf, reg_name reg);
void write_register(reg_file* rf, reg_name reg, int32_t value);

#endif // REGISTER_H