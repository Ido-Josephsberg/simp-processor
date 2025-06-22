// Defines and includes
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "register.h"
#include "simp_helpers.h"

/////////////////////////////////////////// [TODO] /////////////////////////////////////////
// Todos:
//		
//		* Consider using a more robust error handling mechanism instead of just printing errors.
//
//
/////////////////////////////////////////// [TODOS] /////////////////////////////////////////


FILE* checked_fopen(const char* path, const char* mode) {
	/*
	 @brief: This function opens a file and checks if it was opened successfully.

	 @param: path - a pointer to a path string
	 @param: mode - char or string representing the mode in which to open the file (e.g., "r", "w", "a", etc.)

	 @return: file - pointer if the file was opened successfully, or NULL if it fails.
	 */

	FILE* file = fopen(path, mode);
	if (file == NULL) {
		fprintf("Error: failed to open file in path %s", path);
		return NULL;
	}
	return file;
}

int write_str_to_file(FILE* file, char* str) {
	/*
	 @brief: This function writes a string to a file.

	 @param: file - pointer to the file where the string will be written.
	 @param: str - a pointer to a string that will be written to the file.

	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */

	size_t len = strlen(str);

	// Write the string to the file
	if (fwrite(str, sizeof(char), len, file) != len) {
		return -1; // fclose will be called outside the function.
	}

	return 0;
}


int write_num_to_file(FILE* file, int num) {
	/*
	 @brief: This function writes a number to a file in 8-digit zero-padded hexadecimal format.

	 @param: file - pointer to the file where the num will be written.
	 @param: num - an integer number that will be written to the file in hexadecimal format.

	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */

	 // Write the number in 8-digit zero-padded hexadecimal format
	if (fprintf(file, "%08X", num) < 0) {
		return -1; // fclose will be called outside the function.
	}

	return 0;
}

int write_registers_content_to_file(FILE* file, int32_t* reg_array) {
	/*
	 @brief: This function writes the values of registers R2 to R15 to a file in 8-digit zero-padded hexadecimal format.

	 @param: file - pointer to the file where the register values will be written.
	 @param: reg_array - a pointer to an array of integers representing the register values. The array should contain at least 16 elements,
						 where reg_array[0] is R0, reg_array[1] is R1, and so on up to reg_array[15] which is R15.

	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */

	int num_regs = REG_NUM; // R2 to R15 inclusive

	// Write R2 to R15
	for (int i = 2; i < num_regs; i++) {
		if (fprintf(file, "%08X\n", reg_array[i]) < 0) {
			return -1; // fclose will be called outside the function.
		}
	}
	return 0;
}

