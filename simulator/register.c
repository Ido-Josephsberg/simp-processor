#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <string.h>
#include "simulator.h"
#include "register.h"
#include "write_helpers.h"

// Read from any register
uint32_t read_register(Simulator* sim, reg_name reg) {
    if (reg < 0 || reg >= REG_NUM) {
        return 0;
    }
    return sim->reg_array[reg];
}

// Write only to registers 2 to 15
void write_register(Simulator* sim, reg_name reg, uint32_t value) {
    if (reg <= IMM || reg >= REG_NUM) {
		return; // Ignore writes to invalid registers
    }
    sim->reg_array[reg] = value;
}

// Write to IMM register 
void write_register_imm(Simulator* sim, uint32_t value) {
    sim->reg_array[IMM] = value;
}

// Write the regout file with the current registers content
void write_regout_file_wrapper(Simulator* sim, output_paths* path) {
	char* regout_path = path->regout_path;

	// Open the output file for writing
    FILE* regout_file = checked_fopen(regout_path, "w");
    if (regout_file == NULL) {
		free_simulator(sim);
        printf("Error opening file %s for writing\n", regout_path);
        exit(FILE_ERROR);
	}

	// Write the registers content to the file. If it fails, notify the user, close the file, free the simulator, and exit with an error code.
    if (write_registers_content_to_file(regout_file, sim->reg_array) != 0) {
        printf("Error writing registers content to file %s\n", regout_path);
        fclose(regout_file);
        free_simulator(sim);
		exit(FILE_ERROR);
	}

    // Close the file
    fclose(regout_file);
}
 