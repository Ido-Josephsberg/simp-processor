#include "simulator.h"
#include "leds_handler.h"
#include "io_register.h"


static void add_curr_data_to_leds(Simulator* sim) {
	/*
	Add the current cycle and the status of the LEDs register to the leds_str trace string.
	sim: Pointer to the Simulator instance containing the current state of the simulation.
	*/
	// Create a new line for the leds_str
	char new_line[LEDS_LINE_SIZE];
	// Format the current cycle and the status of the LEDs register into the new line
	sprintf(new_line, "%0" MACRO_TO_STR(CYCLE_NUM_OF_DIGITS) "X %0" MACRO_TO_STR(LEDS_STATUS_NUM_OF_DIGITS) "X\n", sim->cycles, sim->io_reg_arr[LEDS]);
	// Append the new line to the leds_str trace string
	strcat(sim->leds_str.data, new_line);
}

void update_leds(Simulator* sim) {
	/*
	Update the LEDs trace string with the current cycle and the status of the LEDs register. Ensure that the trace string has enough allocated memory to append a new line.
	sim: Pointer to the Simulator instance containing the current state of the simulation.
	*/

	// Ensure that the trace_str has enough allocated memory to append new line.
	ensure_str_capacity(&(sim->leds_str), LEDS_LINE_SIZE);
	// Update the trace string with the current cycle, pc, instruction, and register values.
	add_curr_data_to_leds(sim);
}
