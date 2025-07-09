#include <stdint.h>
#include "macros.h"
#include "simulator.h"
#include "write_helpers.h"
#include "memory.h"

void init_memory(Simulator* sim) {
    // Initialize the memory to zero
	memset(sim->memory, 0, sizeof(sim->memory));
}

int32_t read_memory(Simulator* sim, int32_t address) {
	if (address < 0 || address >= MEMORY_SIZE) {
		// Handle out-of-bounds access
		return -1; // or some error code
	}
	return sim->memory[address];
}

int write_memory(Simulator* sim, int32_t address, int32_t value) {
	if (address < 0 || address >= MEMORY_SIZE) {
		// Handle out-of-bounds access
		return -1; // or some error code
	}
	sim->memory[address] = value;
	return 0; // success
}

int write_memout_file_wrapper(Simulator* sim, output_paths* paths) {
    char* memout_path = paths->memout_path;

    // Open the output file for writing
    FILE* memout_file = checked_fopen(memout_path, "w");
    if (memout_file == NULL) {
        printf("Error opening file %s for writing\n", memout_path);
        return -1; // Return error code
    }

    // Write the memory content to the file
    if (write_memory_content_to_file(memout_file, sim->memory, sim->max_memory_address) != 0) {
        printf("Error writing memory content to file %s\n", memout_path);
        fclose(memout_file);
        return -1; // Return error code
    }

    // Close the file
    fclose(memout_file);
    return 0; // Return success code
}