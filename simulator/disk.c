#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "write_helpers.h"
#include "disk.h"

int write_diskout_file_wrapper(Simulator* sim, output_paths* paths) {
    /*
	Wrapper function to write the disk content to a file specified in the output_paths structure.
	sim: Pointer to the Simulator structure.
	paths: Pointer to the output_paths structure containing the diskout_path.
    */

	// Get the diskout_path from the paths structure
    char* diskout_path = paths->diskout_path;

    // Open the output file for writing
    FILE* diskout_file = checked_fopen(diskout_path, "w");
    if (diskout_file == NULL) {
		// If the file could not be opened, print an error message and return an error code
        printf("Error opening file %s for writing\n", diskout_path);
        return FILE_ERROR;
    }

    // Write the string content to the file
    if (write_disk_content_to_file(diskout_file, sim->disk) != 0) {
		// If there was an error writing to the file, print an error message and return an error code
        printf("Error writing memory content to file %s\n", diskout_path);
        fclose(diskout_file);
        return FILE_ERROR;
    }
	// Close the file after writing
    fclose(diskout_file);
    return 0;

}
