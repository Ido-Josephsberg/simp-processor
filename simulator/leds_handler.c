#define _CRT_SECURE_NO_WARNINGS
#include "simulator.h"
#include "leds_handler.h"
#include "io_register.h"
#include "write_helpers.h"


static void add_curr_data_to_leds(Simulator* sim) {
	/*
	Add the current cycle and the status of the LEDs register to the leds_str trace string.
	sim: Pointer to the Simulator instance containing the current state of the simulation.
	*/
	// Create a new line for the leds_str
	char new_line[LEDS_LINE_SIZE];
	// Format the current cycle and the status of the LEDs register into the new line
	sprintf(new_line, "%0" MACRO_TO_STR(CYCLE_NUM_OF_DIGITS) "X %0" MACRO_TO_STR(LEDS_STATUS_NUM_OF_DIGITS) "X\n", sim->cycles, sim->io_reg_array[LEDS]);
	// Append the new line to the leds_str trace string
	strcat(sim->leds_str->data, new_line);
}

void update_leds(Simulator* sim) {
	/*
	Update the LEDs trace string with the current cycle and the status of the LEDs register. Ensure that the trace string has enough allocated memory to append a new line.
	sim: Pointer to the Simulator instance containing the current state of the simulation.
	*/

	// Ensure that the leds_str has enough allocated memory to append new line. If the allocation fails, notify the user, free the simulator and exit with an error code.
	if(ensure_str_capacity(sim->leds_str, LEDS_LINE_SIZE) == -1) {
		printf("Error: Failed to allocate memory for LEDs trace string\n");
		free_simulator(sim);
		exit(MEMORY_ERROR);
	}
	// Update the trace string with the current cycle, pc, instruction, and register values.
	add_curr_data_to_leds(sim);
}

int write_leds_file_wrapper(Simulator* sim, output_paths* paths) {
	/*
	Implement the logic to write the LEDs string content to the output file.
	sim: Pointer to the Simulator object.
	paths: Pointer to the output_paths object containing the path to the LEDs file.
	*/

	// Get the path to the LEDs file from the output_paths object
	char* leds_path = paths->leds_path;

	// Open the output file for writing
	FILE* leds_file = checked_fopen(leds_path, "w");
	// Check if the file was opened successfully. If not, free the simulator and exit with an error code.
	if (leds_file == NULL) {
		free_simulator(sim);
		printf("Error opening file %s for writing\n", leds_path);
		exit(FILE_ERROR);
	}
	// Get the LEDs string from the simulator object
	char* leds_string = sim->leds_str->data;

	//write the trace string content to the file. If the write operation fails, free the simulator, close the file, and exit with an error code.
	if (write_str_to_file(leds_file, leds_string) != 0) {
		free_simulator(sim);			
		printf("Error writing LEDs string content to file %s\n", leds_path);
		fclose(leds_file);
		exit(FILE_ERROR);
	}
	// Close the file after writing
	fclose(leds_file);
	return 0;
}
