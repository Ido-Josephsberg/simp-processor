#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "macros.h"
#include "dynamic_mem.h"
#include "simulator.h"
#include "files_handler.h"
#define IRQ_LINE_HEX_DIGITS 3
#define IRQ_ITEM_BYTES 4

void init_input_paths(input_paths* paths, char* argv[]) {
	/*
	Initializes the input paths for memory, disk, and IRQ2 files.
	paths: Pointer to an input_paths structure that will hold the paths.
	argv[]: An array of command line arguments where 
		- argv[1] is memory input path,
		- argv[2] is disk input path,
		- argv[3] is IRQ2 input path.
	*/
	paths->memin_path = argv[1];
	paths->diskin_path = argv[2];
	paths->irq2in_path = argv[3];
}

void init_output_paths(output_paths* paths, char* argv[]) {
	/*
	Initializes the output paths for various output files.
	paths: Pointer to an output_paths structure that will hold the paths.
	argv[]: An array of command line arguments where 
		- argv[4] is memory output path,
		- argv[5] is register output path,
		- argv[6] is trace output path,
		- argv[7] is hardware register trace path,
		- argv[8] is cycles output path,
		- argv[9] is LEDs output path,
		- argv[10] is 7-segment display output path,
		- argv[11] is disk output path,
		- argv[12] is monitor text output path,
		- argv[13] is monitor YUV output path.
	*/
	
	paths->memout_path = argv[4];
	paths->regout_path = argv[5];
	paths->trace_path = argv[6];
	paths->hwregtrace_path = argv[7];
	paths->cycles_path = argv[8];
	paths->leds_path = argv[9];
	paths->display7seg_path = argv[10];
	paths->diskout_path = argv[11];
	paths->monitor_txt_path = argv[12];
	paths->monitor_yuv_path = argv[13];
}

uint32_t str_hex_to_uint32(const char* hex_str, int* error) {
	/*
	This function converts a hexadecimal string to a 32-bit unsigned integer. In case of an error, it sets the error flag to -1 and returns 0.
	hex_str: A string representing a hexadecimal number.
	*/

	// Varible to hold the end pointer after conversion and check if the conversion was successful.
	char* endptr;
	// Initialize the error flag to 0.
	*error = 0; 
	// Convert the hexadecimal string to a long unsigned int.
	long unsigned int tmp_line_number = strtoul(hex_str, &endptr, 16);

	// Check if the conversion was successful. If it wasn't, notify the user, set the error flag to -1, and return 0.
	if (*endptr != '\0') {
		printf("Error: Invalid hexadecimal string '%s'\n", hex_str);
		*error = -1; 
		return 0;
	}
	// Check if the converted value is within the range of a 32-bit unsigned integer. If it exceeds the range, notify the user and exit.
	if (tmp_line_number > MAX_UINT_32) {
		printf("Error: Invalid input: Got number larger than 32 bits.\n");
		exit(INPUT_ERROR);
	}
	// Return the converted value as a 32-bit unsigned integer.
	return (uint32_t)tmp_line_number;
}

int read_memin_file(input_paths* paths, uint32_t mem[], int* max_address) {
	/*
	Reads the memory input file (memin) and fills the memory array with values. Returns 0 on success or -1 on error.
	paths: Pointer to an input_paths structure that holds the path to the memory input file.
	mem: A memory array where the values will be stored.
	max_address: Pointer to an integer that will hold the maximum address used in the memory.
	*/

	// Variable to hold error status for hexadecimal conversion.
	int error = 0; 

	// Open the memory input file for reading.
	FILE* memin_file = fopen(paths->memin_path, "r");
	// Check if the file was opened successfully. If not, notify the user and return -1.
	if (!memin_file) {
		printf("Error opening memin file");
		return -1;
	}
	// Initialize memory index.
	int current_address = 0;
	// Define a buffer to hold each line read from the file.
	char line_buffer[MEMORY_LINE_HEX_DIGITS + 2];
	// Read the memin file line by line.
	while (fgets(line_buffer, sizeof(line_buffer), memin_file) != NULL) {
		// Check if the current address is within bounds. If it exceeds the memory size, notify the user, close the file and return -1.
		if (current_address >= MEMORY_SIZE) {
			printf("Error: Memory address out of bounds.\n");
			fclose(memin_file);
			return -1;
		}
		// Remove the newline character from the line buffer if it exists.
		if (line_buffer[strlen(line_buffer) - 1] == '\n') {
			line_buffer[strlen(line_buffer) - 1] = '\0';
		}
		// Convert the line from hexadecimal to uint32_t and store it in the memory array.
		mem[current_address] = str_hex_to_uint32(line_buffer, &error);
		// Check if there was an error during conversion. If so, close the file and return -1.
		if (error == -1) {
			fclose(memin_file);
			return -1;
		}
		// If the current address is greater than the maximum address modified so far and the value at that address is not zero, update the maximum address.
		if (current_address > *max_address && mem[current_address] != 0) {
			*max_address = current_address;
		}

		// Increment the current address for the next iteration.
		current_address++;
	}
	// Close the memin file.
	fclose(memin_file);
	return 0;
}

int read_diskin_file(input_paths* paths, uint32_t disk[][DISK_ROWS]) {
	/*
	Reads the disk input file and fills the disk array with values - by sector and row. Returns 0 on success or -1 on error.
	paths: Pointer to an input_paths structure that holds the path to the disk input file.
	disk: A 2D array representing the disk, where each sector contains a number of rows.
	*/

	// Variable to hold error status for hexadecimal conversion.
	int error = 0;
	// Open the disk input file for reading.
	FILE* diskin_file = fopen(paths->diskin_path, "r");
	// Check if the file was opened successfully. If not, notify the user and return -1.
	if (!diskin_file) {
		printf("Error opening diskin file");
		return -1;
	}
	// Initialize disk indices.
	int current_sector = 0;
	int current_row = 0;
	// Read the disk file line by line and convert each line from hexadecimal to uint32_t.
	char line_buffer[DISK_LINE_HEX_DIGITS + 2];
	// Read the diskin file line by line.
	while (fgets(line_buffer, sizeof(line_buffer), diskin_file) != NULL) {
		// Check if the indices are within bounds. If they exceed the disk size, notify the user, close the file and return -1.
		if (current_sector >= DISK_SECTORS || current_row >= DISK_ROWS) {
			printf("Error: Disk indices out of bounds.\n");
			fclose(diskin_file);
			return -1;
		}
		// Remove the newline character from the line buffer if it exists.
		if (line_buffer[strlen(line_buffer) - 1] == '\n') {
			line_buffer[strlen(line_buffer) - 1] = '\0';
		}
		// Convert the line from hexadecimal to uint32_t and store it in the disk array.
		disk[current_sector][current_row] = str_hex_to_uint32(line_buffer, &error);
		// Check if there was an error during conversion. If so, close the file and return -1.
		if (error == -1) {
			fclose(diskin_file);
			return -1;
		}
		// Increment the current row for the next iteration.
		current_row++;
		// If the current row exceeds the number of rows, reset it and increment the sector.
		if (current_row >= DISK_ROWS) {
			current_row = 0;
			current_sector++;
		}
	}
	// Close the diskin file.
	fclose(diskin_file);
	return 0;
}

int read_irq2in_file(input_paths* paths, DynamicMem* irq2in, int* num_of_cycles) {
	/*
	Reads the IRQ2 input file and fills the irq2in dynamic memory structure. Returns 0 on success or -1 on error.
	paths: Pointer to an input_paths structure that holds the path to the IRQ2 input file.
	irq2in: A pointer to a DynamicMem structure that will hold the IRQ2 values.
	num_of_cycles: Pointer to an integer that will hold the number of cycles read from the IRQ2 input file.
	*/

	// Open the IRQ2 input file for reading. If it fails, notify the user and return -1.
	FILE* irq2in_file = fopen(paths->irq2in_path, "r");
	if (!irq2in_file) {
		printf("Error opening irq2in file");
		return -1;
	}
	// Initialize IRQ index.
	int current_irq = 0;
	// Initialize a buffer to hold each line read from the file.
	char line_buffer[IRQ_LINE_HEX_DIGITS + 2];
	// Read the IRQ file line by line.
	while (fgets(line_buffer, sizeof(line_buffer), irq2in_file) != NULL) {
		// Check if the current IRQ index is within bounds.
		if (current_irq * IRQ_ITEM_BYTES >= irq2in->alloc_size) {
			// If it exceeds the allocated size, reallocate memory for the irq2in structure.
			if (dynamic_mem_reallocate(irq2in) == -1) {
				// If reallocation fails, close the file and return -1.
				fclose(irq2in_file);
				return -1;
			}
		}
		// Remove the newline character from the line buffer if it exists.
		if (line_buffer[strlen(line_buffer) - 1] == '\n') {
			line_buffer[strlen(line_buffer) - 1] = '\0';
		}
		// Insert the IRQ value into the dynamic memory structure.
		((uint32_t*)(irq2in->data))[current_irq] = atoi(line_buffer);
		(*num_of_cycles)++;
		// Increment the current IRQ index for the next iteration.
		current_irq++;
	}
	// Close the irq2in file.
	fclose(irq2in_file);
	return 0;
}



