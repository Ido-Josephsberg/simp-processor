#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "assembler.h"
#include "file_handler.h"

// The assembler assumes that the given input is valid according to the specifications.

int main(int argc, char* argv[]) {
	if (argc < 3) { // Just in case - even though we assume valid input
		printf("Error Missing argument (main): %s <input_file> <output_file>\n", argv[0]);
		return -1;
	}
	// Open the assembly program file for reading
	FILE* asm_program = checked_fopen(argv[1], "r");
	if (asm_program == NULL) {
		printf("Error opening file (main): %s\n", argv[1]);
		return -1;
	}
	// Initialize line array and label dictionary array
	Line line_array[MAX_MEM_LINES]; // Array to hold parsed lines
	Label label_dict[MAX_MEM_LINES]; // Array to hold label names & addresses

	// Perform the first pass to parse the assembly file and fill the line array and label dictionary array
	int asm_line_count = 0;
	int label_count = 0;
	if (first_pass(asm_program, line_array, label_dict, &asm_line_count, &label_count) < 0) {
		printf("Error (main): failed in first pass: %s\n", argv[1]);
		fclose(asm_program);
		return -1;
	}
	// Check if the first pass was successful
	if (asm_line_count < 0 || label_count < 0) {
		printf("Error in first pass: %s\n", argv[1]);
		fclose(asm_program);
		return -1;
	}
	
	// Generate the memory input array initialized to zero
	uint32_t memin[MAX_MEM_LINES] = { 0 }; // Memory input array to hold the final machine code
	unsigned int word_is_zero_location[MAX_MEM_LINES] = { 0 }; // Array to track if a word line data is zero

	// Perform the second pass to fill the imm32 field in Imm32 lines
	if (second_pass(line_array, asm_line_count, label_dict, label_count, memin, word_is_zero_location) < 0) {
		printf("Error in second pass: %s\n", argv[1]);
		fclose(asm_program);
		return -1;
	}
	//Find finel memin length
	int memin_last_nonzero = find_last_nonzero_location(memin, word_is_zero_location);
	if (memin_last_nonzero < 0) {
		printf("Error finding last non-zero location in memin array\n");
		fclose(asm_program);
		return -1;
	}
	int memin_len = memin_last_nonzero + 1; // Length of the memin array up to the last non-zero element

	// Open the output file to write the memin content
	FILE* output_file = checked_fopen(argv[2], "w");
	if (output_file == NULL) {
		printf("Error opening output file: %s\n", argv[2]);
		fclose(asm_program);
		return -1;
	}
	
	// Write the memin array to the output file
	if (write_memin_to_file(output_file, memin, memin_len) < 0) {
		printf("Error writing to output file: %s\n", argv[2]);
		fclose(asm_program);
		fclose(output_file);
		return -1;
	}
	fclose(asm_program);
	fclose(output_file);
	printf("Assembly completed successfully. Output written to: %s\n", argv[2]);

	return 0;
}