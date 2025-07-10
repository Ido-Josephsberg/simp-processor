// Defines and includes
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "macros.h"
#include "write_helpers.h"
#include "simulator.h"



FILE* checked_fopen(const char* path, const char* mode) {
	/*
	 @brief: This function opens a file and checks if it was opened successfully.

	 @param: path - a pointer to a path string
	 @param: mode - char or string representing the mode in which to open the file (e.g., "r", "w", "a", etc.)

	 @return: file - pointer if the file was opened successfully, or NULL if it fails.
	 */

	FILE* file = fopen(path, mode);
	if (file == NULL) {
		return NULL;
	}
	return file;
}

int write_str_to_file(FILE* file, char* str) {
	/*
	 @brief: This function writes a string to a file.
			 - relevant to functions: write_trace, write_hwregtrace, write_leds, write_display7seg, write_monitor_txt, write_monitor_yuv
				(6 in total)

	 @param: file - pointer to the file where the string will be written.
	 @param: str - a pointer to a string that will be written to the file.

	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */
	if (str == NULL || file == NULL) {
		return -1; // Error: file pointer is NULL
	}

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
			 - relevant to functions: write_cycles (1 in total)

	 @param: file - pointer to the file where the num will be written.
	 @param: num - an integer number that will be written to the file in hexadecimal format.

	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */

	if (file == NULL) {
		return -1; // Error: file pointer is NULL
	}
	
	// Write the number in 8-digit zero-padded hexadecimal format
	if (fprintf(file, "%08X\n", num) < 0) {
		return -1; // fclose will be called outside the function.
	}

	return 0;
}

int write_registers_content_to_file(FILE* file, int32_t* reg_array) {
	/*
	 @brief: This function writes the values of registers R2 to R15 to a file in 8-digit zero-padded hexadecimal format.
			 - relevant to functions: write_registers (1 in total)

	 @param: file - pointer to the file where the register values will be written.
	 @param: reg_array - a pointer to an array of integers representing the register values. The array should contain at least 16 elements,
						 where reg_array[0] is R0, reg_array[1] is R1, and so on up to reg_array[15] which is R15.

	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */
	if (reg_array == NULL || file == NULL)
		return -1; // Error: file pointer is NULL

	int num_regs = REG_NUM; // R2 to R15 inclusive

	// Write R2 to R15
	for (int i = 2; i < num_regs; i++) {
		if (fprintf(file, "%08X\n", reg_array[i]) < 0) {
			return -1; // fclose will be called outside the function.
		}
	}
	return 0;
}

int write_memory_content_to_file(FILE* file, uint32_t* memory, int maximum_address) {
	/*
	 @brief: This function writes the content of a memory array to a file in 8-digit zero-padded hexadecimal format.
			 - relevant to functions: write_memout (1 in total)

	 @param: file - pointer to the file where the memory content will be written.
	 @param: memory - a pointer to an array of unsigned 32-bit integers representing the memory content.
	 @param: memory_size - the size of the memory array in bytes.

	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */
	if (memory == NULL || file == NULL)
		return -1; // Error: file pointer is NULL

	// Write memory content to the file
	for (int32_t i = 0; i <= maximum_address; i++) {
		if (fprintf(file, "%08X\n", memory[i]) < 0) {
			return -1; // fclose will be called outside the function.
		}
	}
	return 0;
}

int write_disk_content_to_file(FILE* file, uint32_t disk[][DISK_ROWS]) {
	/*
	 @brief: This function writes the content of a diskout 2D array to a file in 8-digit zero-padded hexadecimal format.
			 - it writes the full diskout content, whether it is zero or not.
	 @param: file - pointer to the file where the diskout content will be written.
	 @param: disk - a 2D array of unsigned 32-bit integers representing the diskout content.
	 @param: diskout_size - the size of the diskout array in bytes.
	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */
	
	 // Check if the file pointer and disk pointer are not NULL
	if (disk == NULL || file == NULL) {
		return -1; // Error: file pointer is NULL
	}

	// Write disk content to the file
	for (int i = 0; i < DISK_SECTORS; i++) {
		for (int j = 0; j < DISK_ROWS; j++) {
			// Write the number in 8-digit zero-padded hexadecimal format
			if (fprintf(file, "%08X\n", disk[i][j]) < 0) {
				return -1; // fclose will be called outside the function.
			}
		}
	}
	return 0;
}

int write_monitor_content_to_file(FILE* file, uint8_t monitor[][PIXEL_PER_ROW_COL], int max_pixel_index[]) {
	/*
	 @brief: This function writes the content of a monitor 2D array to a file in 8-digit zero-padded hexadecimal format.
			 - it writes the full monitor content, whether it is zero or not.
	 @param: file - pointer to the file where the diskout content will be written.
	 @param: diskout - a pointer to an array of unsigned 8-bit integers representing the diskout content.
	 @param: diskout_size - the size of the diskout array in bytes.
	 @return: int - returns 0 on success, or -1 if an error occurs (e.g., if the file cannot be opened or written to).
	 */

	 // Check if the file pointer and disk pointer are not NULL
	if (monitor == NULL || file == NULL) {
		return -1; // Error: file pointer is NULL
	} 
	int col_upper_bound = PIXEL_PER_ROW_COL;
	// Write disk content to the file
	for (int i = 0; i <= max_pixel_index[0]; i++) {
		if (i == max_pixel_index[0]) {
			col_upper_bound = max_pixel_index[1] + 1; // Last row may not be full
		}
		for (int j = 0; j < col_upper_bound; j++) { 
			// Write the number in 8-digit zero-padded hexadecimal format
			if (fprintf(file, "%02X\n", monitor[i][j]) < 0) {
				return -1; // fclose will be called outside the function.
			}
		}
	}
	return 0;
}

int write_monitor_content_to_yuv_file(FILE* file, uint8_t monitor[][PIXEL_PER_ROW_COL]) {
	/*
	 @brief: This function writes the content of a monitor 2D array to a file as raw bytes (YUV format).
	 @param: file - pointer to the file where the monitor content will be written.
	 @param: monitor - a 2D array of unsigned 8-bit integers representing the monitor content.
	 @return: int - returns 0 on success, or -1 if an error occurs.
	 */

	 // Check if the file pointer and disk pointer are not NULL
	if (monitor == NULL || file == NULL) {
		return -1; // Error: file pointer is NULL
	}
	// Write the entire monitor buffer as raw bytes (YUV)
	size_t rows = PIXEL_PER_ROW_COL;
	size_t cols = PIXEL_PER_ROW_COL;
	size_t written = fwrite(monitor, sizeof(uint8_t), rows * cols, file);
	if (written != rows * cols) {
		return -1;
	}
	return 0;
}