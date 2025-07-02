#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "register.h"
#include "io_register.h"
#include "memory.h"
#include "dynamic_str.h"

#define DISK_SECTORS 128            // Number of sectors in hard disk           --> Remove this if there's a disk file to include here
#define DISK_ROWS 128               // Number of rows in hard disk's sector     --> Remove this if there's a disk file to include here

typedef struct Simulator {
    bool is_running;
    int32_t pc;                                 // Program Counter
	int32_t inst;                               // Current instruction
	reg_name rd, rs, rt;                        // Register names for destination, source, and target
	int32_t reg_array[REG_NUM];                 // Array of registers - use dedicated register.c functions to access these registers
	int32_t io_reg_array[IO_REG_NUM];           // Array of io registers - use dedicated io_register.c functions to access these registers (must)
    int32_t memory[MEMORY_SIZE];                // Memory array
    int32_t cycles;                             // Cycle count
    int32_t disk[DISK_SECTORS][DISK_ROWS];      // Hard disk represented as a 2D array
	DynamicString trace_str, hwregtrace_str, leds_str, seg7display_str;    // Dynamic strings for output files.
} Simulator;

// Simulator Related Functions
// TODO # TODO # BOM
// e.g. initialize_simulator (init reg, io_regs, etc), execute_instruction, etc.

#endif // SIMULATOR_H
