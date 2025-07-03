#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include <stdlib.h>
#include "files_handler.h"

#define REG_NUM 16

// forward declaration of Simulator structure
struct Simulator;
typedef struct Simulator Simulator;

typedef enum {
	ZERO,	// 0
	IMM,	// 1
	V0,		// 2
    A0,     // 3
    A1,     // 4
    A2,     // 5
    A3,     // 6
    T0,     // 7
    T1,     // 8
    T2,     // 9
    S0,     // 10
    S1,     // 11
    S2,     // 12
    GP,     // 13
    SP,     // 14
    RA      // 15
} reg_name;

// Register Related Functions
int32_t read_register(Simulator* sim, reg_name reg);
void write_register(Simulator* sim, reg_name reg, int32_t value);
void write_register_imm(Simulator* sim, int32_t value);
int write_regout_file_wrapper(Simulator* sim, output_paths* path);

#endif // REGISTER_H