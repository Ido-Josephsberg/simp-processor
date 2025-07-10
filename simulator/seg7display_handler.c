#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "macros.h"
#include "io_register.h"
#include "simulator.h"
#include "write_helpers.h"
#include "dynamic_mem.h"
#include "seg7display_handler.h"


static void add_curr_data_to_7seg(Simulator* sim) {
	/*
	Add the current cycle and the status of the 7 segment display display7seg trace string.
	sim: Pointer to the Simulator instance containing the current state of the simulation.
	*/

	// Create a new line for the display7seg_str
	char new_line[SEG7DISPLAY_LINE_SIZE];
	// Format the current cycle and the status of the 7 segment display into the new line
	sprintf(new_line, "%0" MACRO_TO_STR(CYCLE_NUM_OF_DIGITS) "X %0" MACRO_TO_STR(SEG7DISPLAY_STATUS_NUM_OF_DIGITS) "X\n", sim->cycles, sim->io_reg_array[DISPLAY7SEG]);
	// Append the new line to the seg7display_str trace string
	strcat(sim->seg7display_str->data, new_line);
}

void update_7seg(Simulator* sim) {
	/*
	Update the 7 segment display trace string with the current cycle and the status of the 7 segment display. Ensure that the trace string has enough allocated memory to append a new line.
	sim: Pointer to the Simulator instance containing the current state of the simulation.
	*/

	// Ensure that the seg7display_str has enough allocated memory to append new line. If the memory allocation fails, print an error message and exit the program.
	if(ensure_str_capacity(sim->seg7display_str, SEG7DISPLAY_LINE_SIZE) == -1) {
		printf("Error: Memory allocation failed while updating the 7 segment display trace string.\n");
		free_simulator(sim);
		exit(EXIT_FAILURE);
	}
	// Update the trace string with the current cycle and the status of the 7 segment display.
	add_curr_data_to_7seg(sim);
}

void write_display7seg_file_wrapper(Simulator* sim, output_paths* paths) {
	/*
	Function to write the 7-segment display trace string to a specified output file.
	sim: Pointer to the Simulator object.
	paths: Pointer to the output_paths structure containing file paths for outputs.
	*/

	// Get the path for the display7seg output file from the paths structure
	char* display7seg_path = paths->display7seg_path;

	// Open the output file for writing
	FILE* display7seg_file = checked_fopen(display7seg_path, "w");
	// Check if the file was opened successfully. If not, print an error message, free the simulator and exit the program.
	if (display7seg_file == NULL) {
		printf("Error opening file %s for writing\n", display7seg_path);
		free_simulator(sim);
		exit(FILE_ERROR);
	}
	// Extract the display7seg string from the simulator.
	char* display7seg_string = sim->seg7display_str->data;

	// Write the trace string content to the file. If the write operation fails, print an error message, close the file, free the simulator and exit the program.
	if (write_str_to_file(display7seg_file, display7seg_string) != 0) {
		printf("Error writing 7-segment display trace string to file %s\n", display7seg_path);
		fclose(display7seg_file);
		free_simulator(sim);
		exit(FILE_ERROR);
	}
	// Close the file after writing
	fclose(display7seg_file);
}