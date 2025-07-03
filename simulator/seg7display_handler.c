#define _CRT_SECURE_NO_WARNINGS
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

int write_display7seg_file_wrapper(Simulator* sim, output_paths* paths) {

	char* display7seg_path = paths->display7seg_path;

	// Open the output file for writing
	FILE* display7seg_file = checked_fopen(display7seg_path, "w");
	if (display7seg_file == NULL) {
		printf("Error opening file %s for writing\n", display7seg_path);
		return -1; // Return error code
	}
	//*TODO: Implement the logic to extract the display7seg_string from the simulator.
	char* display7seg_string = sim->seg7display_str.data;

	//write the trace string content to the file
	if (write_str_to_file(display7seg_file, display7seg_string) != 0) {
		printf("Error writing 7-segment display trace string to file %s\n", display7seg_path);
		fclose(display7seg_file);
		return -1; // Return error code
	}

	fclose(display7seg_file);
	return 0;

}