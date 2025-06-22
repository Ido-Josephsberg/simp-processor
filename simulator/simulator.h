#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "register.h"
#include "io_register.h"
#include "memory.h"

#define DISK_SECTORS 128            // Number of sectors in hard disk           --> Remove this if there's a disk file to include here
#define DISK_ROWS 128               // Number of rows in hard disk's sector     --> Remove this if there's a disk file to include here

typedef struct {
    bool is_running;
    int32_t pc;                                 // Program Counter
    int32_t reg_array[REG_NUM];                 // Array of registers
	// io_reg_pntr_array                                                            // TODO # TODO # BOM
    int32_t memory[MEMORY_SIZE];                // Memory array
    int32_t cycles;                             // Cycle count
    int32_t disk[DISK_SECTORS][DISK_ROWS];      // Hard disk represented as a 2D array
} Simulator;

// Simulator Related Functions
// TODO # TODO # BOM
// e.g. initialize_simulator, execute_instruction, etc.

#endif // SIMULATOR_H
