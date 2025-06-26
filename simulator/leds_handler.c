#include "simulator.h"
#include "leds_handler.h"
#include "io_register.h"


static void add_curr_data_to_leds(Simulator* sim) {
	// Create a new line for the leds_str
	char new_line[LEDS_LINE_SIZE];
	sprintf(new_line, "%0" MACRO_TO_STR(CYCLE_NUM_OF_DIGITS) "X %0" MACRO_TO_STR(LEDS_STATUS_NUM_OF_DIGITS) "X\n", sim->cycles, sim->io_reg_arr[LEDS]);
	strcat(sim->leds_str.data, new_line);
}

void update_leds(Simulator* sim) {


	// Ensure that the trace_str has enough allocated memory to append new line.
	ensure_str_capacity(&(sim->leds_str), LEDS_LINE_SIZE);
	// Update the trace string with the current cycle, pc, instruction, and register values.
	add_curr_data_to_leds(sim);
	
}