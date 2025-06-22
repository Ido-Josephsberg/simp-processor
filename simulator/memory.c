#include <stdint.h>
#include "memory.h"
#include "simulator.h"


int32_t read_memory(Simulator* sim, int32_t address) {
	if (address < 0 || address >= MEMORY_SIZE) {
		// Handle out-of-bounds access
		return -1; // or some error code
	}
	return sim->memory[address];
}

write_memory(Simulator* sim, int32_t address, int32_t value) {
	if (address < 0 || address >= MEMORY_SIZE) {
		// Handle out-of-bounds access
		return -1; // or some error code
	}
	sim->memory[address] = value;
	return 0; // success
}