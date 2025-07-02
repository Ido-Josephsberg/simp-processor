#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "disk.h"

int write_diskout_file_wrapper(Simulator* sim, output_paths* paths) {

    char* diskout_path = paths->display7seg_path;

    // Open the output file for writing
    FILE* diskout_file = checked_fopen(diskout_path, "w");
    if (diskout_file == NULL) {
        printf("Error opening file %s for writing\n", diskout_path);
        return -1; // Return error code
    }
    // Extract the display7seg_string from the simulator.
    int32_t** diskout = sim->disk;

    //write the string content to the file
    if (write_disk_content_to_file(diskout_file, diskout) != 0) {
        printf("Error writing memory content to file %s\n", diskout_path);
        fclose(diskout_file);
        return -1; // Return error code
    }

    fclose(diskout_file);
    return 0;

}
