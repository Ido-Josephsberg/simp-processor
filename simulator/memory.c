#include <stdint.h>
#include "macros.h"
#include "simulator.h"
#include "write_helpers.h"
#include "memory.h"

uint32_t read_memory(Simulator* sim, int32_t address) {
    /*
	Reads a 32-bit value from the memory at the specified address.
    */

	// Check if the address is within the valid range of memory. If not, free the simulator, notify and exit with an error code.
	if (address < 0 || address >= MEMORY_SIZE) {
		free_simulator(sim);
		printf("Error: Memory access out of bounds at address %d\n", address);
        exit(INPUT_ERROR);
	}
	return sim->memory[address];
}

void write_memory(Simulator* sim, int32_t address, uint32_t value) {
    /*
	Writes a 32-bit value to the memory at the specified address.
	sim: Pointer to the simulator object.
	address: The address in memory where the value should be written.
	value: The 32-bit value to write to memory.
    */

	// Check if the address is within the valid range of memory. If not, notify, free simulator and exit with an error code.
	if (address < 0 || address >= MEMORY_SIZE) {
		printf("Error: Memory access out of bounds at address %d\n", address);
		free_simulator(sim);
		exit(INPUT_ERROR);
	}
	// Write the value to the specified address in memory
	sim->memory[address] = value;
}

void write_memout_file_wrapper(Simulator* sim, output_paths* paths) {
    /*
	Writes the memory content to a file specified in the output paths.
	sim: Pointer to the simulator object.
	paths: Pointer to the output paths structure containing the file path for memory output.
    */

	// Get the memory output file path from the paths structure
    char* memout_path = paths->memout_path;

    // Open the output file for writing
    FILE* memout_file = checked_fopen(memout_path, "w");
	// Check if the file was opened successfully. If not, free the simulator, notify and exit with an error code.
    if (memout_file == NULL) {
        printf("Error opening file %s for writing\n", memout_path);
		free_simulator(sim);
		exit(FILE_ERROR);
    }

	// Write the memory content to the file. If the write operation fails, notify, free the simulator, close the file and exit with an error code.
    if (write_memory_content_to_file(memout_file, sim->memory, sim->max_memory_address) != 0) {
        printf("Error writing memory content to file %s\n", memout_path);
        fclose(memout_file);
		free_simulator(sim);
		exit(FILE_ERROR);
    }

    // Close the file
    fclose(memout_file);
}