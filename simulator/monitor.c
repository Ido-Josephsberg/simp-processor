#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "write_helpers.h"
#include "monitor.h"


void write_monitor_files_wrapper(Simulator* sim, output_paths* paths, int max_monitor_pixel[]) {
    /*
	Wrapper function to write the monitor content to files.
	sim: Pointer to the Simulator object.
	paths: Pointer to the output_paths structure containing file paths for monitor output.
    */

	// Get the paths for the monitor output files
    char* monitor_txt_path = paths->monitor_txt_path;
    char* monitor_yuv_path = paths->monitor_yuv_path;

    // Open the output files for writing. 
    FILE* monitor_txt_file = checked_fopen(monitor_txt_path, "w");
    if (monitor_txt_file == NULL) {
        printf("Error opening file %s for writing\n", monitor_txt_path);
		free_simulator(sim);
        exit(FILE_ERROR);
    }

    FILE* monitor_yuv_file = checked_fopen(monitor_yuv_path, "wb");
    if (monitor_yuv_file == NULL) {
        printf("Error opening file %s for writing\n", monitor_yuv_path);
		free_simulator(sim); 
        fclose(monitor_txt_file);
		exit(FILE_ERROR);
    }

	// Write the monitor string content to the txt file. If the file writing fails, print an error message, close the files, free the simulator and exit.
    if (write_monitor_content_to_file(monitor_txt_file, sim->monitor, max_monitor_pixel) != 0) {
        printf("Error writing memory content to file %s\n", monitor_txt_path);
        fclose(monitor_txt_file);
		fclose(monitor_yuv_file);
		free_simulator(sim);
		exit(FILE_ERROR); 
    }
	// Close the text file after writing
    fclose(monitor_txt_file);


    // Write the monitor content to the YUV file using the appropriate function for 2D arrays. If the file writing fails, print an error message, close the file, free the simulator and exit.
    if (write_monitor_content_to_yuv_file(monitor_yuv_file, sim->monitor) != 0) {
        printf("Error writing monitor content to file %s\n", monitor_yuv_path);
        fclose(monitor_yuv_file);
		exit(FILE_ERROR);
    }

	// Close the YUV file after writing
    fclose(monitor_yuv_file);
}
