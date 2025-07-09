#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "macros.h"
#include "dynamic_mem.h"
#include "files_handler.h"
#define IRQ_LINE_HEX_DIGITS 3
#define IRQ_ITEM_BYTES 4

void init_input_paths(input_paths* paths, char* argv[]) {
	paths->memin_path = argv[1];
	paths->diskin_path = argv[2];
	paths->irq2in_path = argv[3];
}

void init_output_paths(output_paths* paths, char* argv[]) {
	// Initialize all paths to NULL or empty strings
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

uint32_t str_hex_to_uint32(const char* hex_str) {
	/*
	This function converts a hexadecimal string to a 32-bit unsigned integer.
	*/

	char* endptr;
	// Convert the hexadecimal string to a long unsigned int.
	long unsigned int tmp_line_number = strtoul(hex_str, &endptr, 16);
	// Check if the conversion was successful.
	if (*endptr != '\0') {
		printf("Error: Invalid hexadecimal string '%s'\n", hex_str);
		exit(INPUT_ERROR);
	}
	// Check if the converted value is within the range of a 32-bit unsigned integer.
	if (tmp_line_number > MAX_UINT_32) {
		printf("Error: Invalid input: Got number larger than 32 bits.\n");
		exit(INPUT_ERROR);
	}
	// Return the converted value as a 32-bit unsigned integer.
	return (uint32_t)tmp_line_number;
}

void read_memin_file(input_paths* paths, uint32_t mem[], int* max_address) {
	/*
	Reads the memory input file and fills the memory array with values.
	*/

	// Open the memory input file for reading.
	FILE* memin_file = fopen(paths->memin_path, "r");
	// Check if the file was opened successfully.
	if (!memin_file) {
		printf("Error opening memin file");
		exit(FILE_ERROR);
	}
	// Initialize memory index.
	int current_address = 0;
	// Read the memory file line by line and convert each line from hexadecimal to uint32_t.
	char line_buffer[MEMORY_LINE_HEX_DIGITS + 2];
	while (fgets(line_buffer, sizeof(line_buffer), memin_file) != NULL) {
		// Check if the current address is within bounds.
		if (current_address >= MEMORY_SIZE) {
			printf("Error: Memory address out of bounds.\n");
			exit(INPUT_ERROR);
		}
		// Remove the newline character from the line buffer if it exists.
		if (line_buffer[strlen(line_buffer) - 1] == '\n') {
			line_buffer[strlen(line_buffer) - 1] = '\0';
		}
		// Convert the line from hexadecimal to uint32_t and store it in the memory array.
		mem[current_address] = str_hex_to_uint32(line_buffer);

		if (current_address > *max_address && mem[current_address] != 0) {
			*max_address = current_address;
		}

		// Increment the current address for the next iteration.
		current_address++;
	}
	// Close the memin file.
	fclose(memin_file);
}

void read_diskin_file(input_paths* paths, uint32_t disk[][DISK_ROWS]) {
	/*
	Reads the disk input file and fills the disk array with values - by sector and row.
	*/

	// Open the disk input file for reading.
	FILE* diskin_file = fopen(paths->diskin_path, "r");
	// Check if the file was opened successfully.
	if (!diskin_file) {
		printf("Error opening diskin file");
		exit(FILE_ERROR);
	}
	// Initialize disk indices.
	int current_sector = 0;
	int current_row = 0;
	// Read the disk file line by line and convert each line from hexadecimal to uint32_t.
	char line_buffer[DISK_LINE_HEX_DIGITS + 2];
	while (fgets(line_buffer, sizeof(line_buffer), diskin_file) != NULL) {
		// Check if the indices are within bounds.
		if (current_sector >= DISK_SECTORS || current_row >= DISK_ROWS) {
			printf("Error: Disk indices out of bounds.\n");
			exit(INPUT_ERROR);
		}
		// Remove the newline character from the line buffer if it exists.
		if (line_buffer[strlen(line_buffer) - 1] == '\n') {
			line_buffer[strlen(line_buffer) - 1] = '\0';
		}
		// Convert the line from hexadecimal to uint32_t and store it in the disk array.
		disk[current_sector][current_row] = str_hex_to_uint32(line_buffer);
		// Increment the current row for the next iteration.
		current_row++;
		// If the current row exceeds the number of rows, reset it and increment the sector.
		if (current_row >= DISK_ROWS) {
			current_row = 0;
			current_sector++;
		}
	}
	// close the diskin file.
	fclose(diskin_file);
}

void read_irq2in_file(input_paths* paths, DynamicMem* irq2in, int* num_of_cycles) {
	FILE* irq2in_file = fopen(paths->irq2in_path, "r");
	if (!irq2in_file) {
		printf("Error opening irq2in file");
		exit(FILE_ERROR);
	}
	// Initialize IRQ index.
	int current_irq = 0;
	// Read the IRQ file line by line.
	char line_buffer[IRQ_LINE_HEX_DIGITS + 2];
	while (fgets(line_buffer, sizeof(line_buffer), irq2in_file) != NULL) {
		// Check if the current IRQ index is within bounds.
		if (current_irq * IRQ_ITEM_BYTES >= irq2in->alloc_size) {
			dynamic_mem_reallocate(irq2in);
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
}



