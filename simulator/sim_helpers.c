#include <stdio.h>
#include <stdint.h>
#include <string.h>

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