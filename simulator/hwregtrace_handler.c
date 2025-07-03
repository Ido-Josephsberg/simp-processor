#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "simulator.h"
#include "sim_helpers.h"
#include "isa_func.h"
#include "io_register.h"
#include "hwregtrace_handler.h"



static void add_curr_data_to_hwregtrace(Simulator* sim) {
	/*
	Update the hwregtrace with the current cycle, indication to whether it was a read or write instruction, the name of the hardware register and the data read from/ written to the register.
	sim: The pointer to the simulator struct that contains the current state of the simulation, including the current cycle, instruction being executed, and the hardware register array.
	*/

	// Declare string that can contain "READ" or "WRITE", including null terminator, a string that can contain the name of 
	// the register which is being read from or written to and a string for the new line.
	char read_write[6], io_reg_name[IO_REG_MAX_NAME_SIZE], new_line[HWREGTRACE_LINE_SIZE];
	// Check if the instruction is a read (IN) or write (OUT) operation.
	if (sim->inst == IN)
		strcpy(read_write, "READ");
	else
		strcpy(read_write, "WRITE");

	// The index of the io register that was involved in the instruction.
	int reg_index = sim->rs + sim->rt;
	// Update the io_reg_name with the name of the relevant hardware register.
	get_io_reg_name(io_reg_name, reg_index);

	// Format the new line as the current cycle written with CYCLE_NUM_OF_DIGITS hex digits, READ/WRITE indication, the name of the relevant register and the data read from him/ written to him in HWREGTRACE_DATA_NUM_OF_DIGITS hex digits.
	sprintf(new_line, "%0" MACRO_TO_STR(CYCLE_NUM_OF_DIGITS) "X %s %s %0" MACRO_TO_STR(HWREGTRACE_DATA_NUM_OF_DIGITS) "X\n", sim->cycles, read_write, io_reg_name, sim->io_reg_arr[reg_index]);
	// Concatenate the new line to hwregtrace_str
	strcat(sim->hwregtrace_str.data, new_line);
}


void update_hwregtrace(Simulator* sim) {
	/*
	Update the hardware register trace string with the current value of a specific hardware register, in the format "CYCLE READ/WRITE NAME DATA"
	sim: The pointer to the simulator struct that contains the current state of the simulation, including the current cycle, instruction being executed, and the hardware register array.
	*/

	// Ensure that the hwregtrace_str has enough allocated memory to append new line.
	ensure_str_capacity(&(sim->hwregtrace_str), HWREGTRACE_LINE_SIZE);
	// Format the hwregtrace string with the cycle written with CYCLE_NUM_OF_DIGITS digits, READ/WRITE indication, and the data.
	add_curr_data_to_hwregtrace(sim);
}