#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include "register.h" //uncomment before merge
#include "register_delete_before_merge.h" //before merge change it to "register.h"

int write_str_to_file(char* output_path, char* str) {
	// This function writes a string to a file specified by output_path.
	size_t len = strlen(str);
	FILE* file = fopen("output_path", "w");

	// Check if file opens successfully
	if (file == NULL)
		return -1;

	// Write the string to the file
	if (fwrite(str, sizeof(char), len, file) != len) {
		fclose(file);
		return -1;
	}

	fclose(file);
	return 0;
}

int write_num_to_file(char* output_path, int num) {
	// This function writes a number to a file specified by output_path in hexadecimal format.
	FILE* file = fopen(output_path, "w");
	
	// Check if file opens successfully
	if (file == NULL)
		return -1;

	// Write the number in 8-digit zero-padded hexadecimal format
	if (fprintf(file, "%08X", num) < 0) {
		fclose(file);
		return -1;
	}

	fclose(file);
	return 0;
}

int write_registers_to_file(char* regout_path, int32_t* reg_array) {
	// This function writes the values of registers R2 - R15 to a file.
	int num_regs = REG_NUM; // R2 to R15 inclusive
	FILE* file = fopen(regout_path, "w");


	// Check if file opens successfully
	if (file == NULL)
		return -1;

	// Write R2 to R15
	for (int i = 2; i < num_regs; i++) {
		if (fprintf(file, "%08X\n", reg_array[i]) < 0) {
			fclose(file);
			return -1;
		}
	}

	fclose(file);
	return 0;
}