#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "macros.h"
#include "register.h"
#include "memory.h"
#include "dynamic_mem.h"


typedef struct Simulator {
	bool is_running;							 // Flag to indicate if the simulator is running
	bool is_pc_changed;							 // Flag to indicate if the program counter has changed due to an instruction (jump or branch instruction)
	bool is_in_irq;								 // Flag to indicate if the simulator is currently in an IRQ (Interrupt Request) state
    uint32_t pc;                                 // Program Counter
	uint32_t inst;                               // Current instruction
	uint8_t opcode;								 // Opcode of the current instruction
	reg_name rd, rs, rt;                         // Register names for destination, source, and target
	int bigimm;									 // Big immediate value (0 or 1)
	uint32_t reg_array[REG_NUM];                 // Array of registers - use dedicated register.c functions to access these registers
	uint32_t io_reg_array[IO_REG_NUM];           // Array of io registers - use dedicated io_register.c functions to access these registers (must)
	int max_memory_address;						 // The maximum memory address that has been witten to.
    uint32_t memory[MEMORY_SIZE];                // Memory array
    uint32_t cycles;                             // Cycle count
    uint32_t disk[DISK_SECTORS][DISK_ROWS];      // Hard disk represented as a 2D array
	int max_monitor_pixel[MONITOR_DIMENSION];	 // Maximum pixel index in the monitor (used for pixel drawing)
	uint8_t monitor[PIXEL_PER_ROW_COL][PIXEL_PER_ROW_COL]; // Monitor represented as a 2D array of pixels
	DynamicMem* trace_str, *hwregtrace_str, *leds_str, *seg7display_str;    // Dynamic strings for output files.
	DynamicMem* irq2_cycles;					 // Dynamic array to store the cycles when IRQ2 was triggered.
	int num_irq2_cycles;						 // Number of cycles when IRQ2 was triggered.
} Simulator;

Simulator* simulator_init(input_paths in_paths);
void free_simulator(Simulator* sim);

#endif // SIMULATOR_H
