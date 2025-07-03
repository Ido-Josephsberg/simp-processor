#include "simulator.h"
#include "sim_helpers.h"
#include "trace_handler.h"




static void add_curr_data_to_trace(Simulator* sim) {
	/*
	Update the trace with the current cycle, program counter, instruction, and register values.
	sim: The pointer to the simulator object containing the current state of the simulation.
	*/

	// Declare a string to hold the new line and a string to hold the formatted register value (1 for space + 1 for null terminator).
	char new_line[TRACE_LINE_SIZE];

	// Insert to the new line string the cycle written with CYCLE_NUM_OF_DIGITS hex digits, pc with PC_NUM_OF_DIGITS hex digits and instruction with INST_NUM_OF_DIGITS hex digits.
	sprintf(new_line, "%0" MACRO_TO_STR(CYCLE_NUM_OF_DIGITS) "X %0" MACRO_TO_STR(PC_NUM_OF_DIGITS) "X %0" MACRO_TO_STR(INST_NUM_OF_DIGITS) "X", sim->cycles, sim->pc, sim->inst);
	// Get the current position in the new line string
	char* cur_pos = new_line + strlen(new_line);
	// Append the register values to the new line string.
	for (int i = 0; i < REG_NUM; i++) {
		// Update the new line with the value of the register at index i, formatted as an REG_STR_SIZE-digit hexadecimal number. update the current position in the new line string to the end of the string.
		cur_pos += sprintf(cur_pos, " %0" MACRO_TO_STR(REG_STR_SIZE) "X", sim->reg_array[i]);
	}
	// Append a newline character to the end of the new line string to separate it from the next entry.
	sprintf(cur_pos, "\n");

	// Append the new line to the trace string.
	strcat(sim->trace_str.data, new_line);
}

void update_trace(Simulator* sim) {
	/*
	Update the trace string with the current cycle, program counter, instruction, and register values, after ensuring that the trace_str has enough allocated memory to append a new line.
	sim: The pointer to the simulator object containing the current state of the simulation.
	*/

	// Ensure that the trace_str has enough allocated memory to append new line.
	ensure_str_capacity(&(sim->trace_str), TRACE_LINE_SIZE);
	// Update the trace string with the current cycle, pc, instruction, and register values.
	add_curr_data_to_trace(sim);
}