#include "simulator.h"
#include "io_register.h"
#include "seg7display_handler.h"


static void add_curr_data_to_7seg(Simulator* sim) {
	/*
	Add the current cycle and the status of the 7 segment display display7seg trace string.
	sim: Pointer to the Simulator instance containing the current state of the simulation.
	*/

	// Create a new line for the display7seg_str
	char new_line[SEG7DISPLAY_LINE_SIZE];
	// Format the current cycle and the status of the 7 segment display into the new line
	sprintf(new_line, "%0" MACRO_TO_STR(CYCLE_NUM_OF_DIGITS) "X %0" MACRO_TO_STR(SEG7DISPLAY_STATUS_NUM_OF_DIGITS) "X\n", sim->cycles, sim->io_reg_arr[DISPLAY7SEG]);
	// Append the new line to the seg7display_str trace string
	strcat(sim->seg7display_str.data, new_line);
}

void update_7seg(Simulator* sim) {
	/*
	Update the 7 segment display trace string with the current cycle and the status of the 7 segment display. Ensure that the trace string has enough allocated memory to append a new line.
	sim: Pointer to the Simulator instance containing the current state of the simulation.
	*/

	// Ensure that the seg7display_str has enough allocated memory to append new line.
	ensure_str_capacity(&(sim->seg7display_str), SEG7DISPLAY_LINE_SIZE);
	// Update the trace string with the current cycle and the status of the 7 segment display.
	add_curr_data_to_7seg(sim);
}
