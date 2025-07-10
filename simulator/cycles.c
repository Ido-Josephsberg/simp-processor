#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "write_helpers.h"
#include "cycles.h"


int write_cycles_file_wrapper(Simulator* sim, output_paths* paths) {
    /*
    Function to write the number of cycles to a specified file.
	sim: Pointer to the Simulator structure containing the cycles count.
	paths: Pointer to the output_paths structure containing the file path for the output file cycles.
    */
    
	// Get the path for the cycles file from the paths structure
    char* cycles_path = paths->cycles_path;

	// Open the output file for writing and check for errors.
    FILE* cycles_file = checked_fopen(cycles_path, "w");
    if (cycles_file == NULL) {
		// If the file could not be opened, print an error message and return an error code.
        printf("Error opening file %s for writing\n", cycles_path);
        return -1;
    }
	// Get the number of cycles from the Simulator structure
    int32_t cycles_num = sim->cycles;

	// Write the cycles content to the file and check for errors.
    if (write_num_to_file(cycles_file, cycles_num) != 0) {
		// If there was an error writing to the file, print an error message, close the file and return an error code.
        printf("Error writing cycles content to file %s\n", cycles_path);
        fclose(cycles_file);
        return FILE_ERROR; 
    }
	// Close the file after writing
    fclose(cycles_file);
    return 0;
}