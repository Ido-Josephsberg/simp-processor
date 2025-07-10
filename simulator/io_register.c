#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "simulator.h"
#include "io_register.h"

int get_io_reg_name(char* dst, io_reg_name reg_index) {
	/*
	Write the name of the io register to dst based on the index. Return 0 on success, -1 on failure.
	dst: Pointer to a string buffer where the name will be written. should be at least IO_REG_MAX_NAME_SIZE characters long.
	reg_index: The index of the io register to copy his name.
	*/

    // Array of io register names ordered by their indices - lowercase.
    const char* hw_reg_names[IO_REG_NUM] = {
        "irq0enable", "irq1enable", "irq2enable", "irq0status", "irq1status", "irq2status", "irqhandler", "irqreturn", 
        "clks", "leds", "display7seg", "timerenable", "timercurrent", "timermax", "diskcmd", "disksector",  
        "diskbuffer", "diskstatus", "reserved18", "reserved19", "monitoraddr", "monitordata", "monitorcmd"
    };
    // Copy the name to dst if index is valid
    if (reg_index >= 0 && reg_index < IO_REG_NUM) {
        strncpy(dst, hw_reg_names[reg_index], IO_REG_MAX_NAME_SIZE);
        return 0;
    }
	// If index is invalid, return -1
	printf("Error: Invalid io register index %d\n", reg_index);
    return -1;
}

int get_io_reg_length(io_reg_name reg_index) {
    /*
    Return the length of the io register based on the index.
    reg_index: The index of the io register to get his length.
    */
    // Array of io register lengths ordered by their indices
    const int hw_reg_lengths[IO_REG_NUM] = {
        1, 1, 1, 1, 1, 1,
        12, 12, 
        32, 32, 32,
        1, 
        32, 32,
        2,
        7,
        12,
        1,
		32, 32,         // Reserved registers
        16,
        8,
        1
    };
    // Return length if index is valid
    if (reg_index >= 0 && reg_index < IO_REG_NUM) {
        return hw_reg_lengths[reg_index];
    }
	return -1; // Invalid index
}

int32_t read_io_reg(Simulator* sim, io_reg_name reg_index) {
    int len = get_io_reg_length(reg_index);
    if (len > 0 && reg_index >= 0 && reg_index < IO_REG_NUM) {
        int32_t mask = (len == 32) ? 0xFFFFFFFF : ((1 << len) - 1);
        return sim->io_reg_array[reg_index] & mask;
    }
    return 0;
}

void write_io_reg(Simulator* sim, io_reg_name reg_index, int32_t value) {
    int len = get_io_reg_length(reg_index);
    if (len > 0 && reg_index >= 0 && reg_index < IO_REG_NUM) {
        int32_t mask = (len == 32) ? 0xFFFFFFFF : ((1 << len) - 1);
        sim->io_reg_array[reg_index] = value & mask;
    }
}