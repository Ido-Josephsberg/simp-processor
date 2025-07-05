#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "assembler.h"

////////////////////////// TODO ////////////////////////////////////
//  # If there is ----------------------------> please take a look
////////////////////////////////////////////////////////////////////

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

int find_last_nonzero_location(uint32_t* memin, unsigned int* word_is_zero_locations) {
	/*
	 * Find the last non-zero location in the memin array.
	 * @param: memin - pointer to the memory input array
	 * @param: memin_len - length of the memin array
	 * @return: int - index of the last non-zero element, or -1 if all elements are zero
	 */
	if (memin == NULL) {
		return -1; // Invalid input
	}
	for (int i = MAX_MEM_LINES - 1; i >= 0; --i) {
		if (memin[i] != 0 || word_is_zero_locations[i] == 1) {
			return i; // Return the index of the last non-zero element
		}
	}
	return -1; // All elements are zero
}

int write_memin_to_file(FILE* file, uint32_t* memin, size_t memin_len) {

    if (file == NULL || memin == NULL) {
		printf("Error (file_handler): Invalid file or memory input\n");
        return -1;
    }

    for (size_t i = 0; i < memin_len; ++i) {
        if (fprintf(file, "%08X\n", memin[i]) < 0) {
			return -1; // Write error - fclose will be called in the main function
        }
    }
    return 0;
}

