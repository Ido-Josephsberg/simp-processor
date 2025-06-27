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
#define PIXEL_PER_ROW_COL 256 // Number of pixels per row/column - monitor file

typedef struct Simulator {
    bool is_running;
    int32_t pc;                                 // Program Counter
	int32_t inst;                               // Current instruction
	reg_name rd, rs, rt;                        // Register names for destination, source, and target
    int32_t reg_array[REG_NUM];                 // Array of registers
    int32_t io_reg_arr[IO_REG_NUM];             // Array of io registers
    int32_t memory[MEMORY_SIZE];                // Memory array
    int32_t cycles;                             // Cycle count
    int32_t disk[DISK_SECTORS][DISK_ROWS];      // Hard disk represented as a 2D array
	int8_t monitor[PIXEL_PER_ROW_COL][PIXEL_PER_ROW_COL]; // Monitor represented as a 2D array of pixels
	DynamicString trace_str, hwregtrace_str, leds_str, seg7display_str;    // Dynamic strings for output files.
} Simulator;

// Simulator Related Functions
// TODO # TODO # BOM
// e.g. initialize_simulator, execute_instruction, etc.

#endif // SIMULATOR_H
