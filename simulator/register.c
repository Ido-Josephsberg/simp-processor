#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include "simulator.h"
#include "register.h"
#include "write_helpers.h"
#include "files_handler.h"


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

int write_regout_file_wrapper(Simulator* sim, output_paths* path) {
	char* regout_path = path->regout_path;

	// Open the output file for writing
    FILE* regout_file = checked_fopen(regout_path, "w");
    if (regout_file == NULL) {
        printf("Error opening file %s for writing\n", regout_path);
        return -1; // Return error code
	}

	//write the registers content to the file
    if (write_registers_content_to_file(regout_file, sim->reg_array) != 0) {
        printf("Error writing registers content to file %s\n", regout_path);
        fclose(regout_file);
        return -1; // Return error code
	}

    // Close the file
    fclose(regout_file);
	return 0; // Return success code
}
 