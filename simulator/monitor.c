#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "monitor.h"

/////////////////////////////////////////// [TODO] ////////////////////////////////////////////////////////////////////////
// Todos:
//		
//      * functions that are not implemented yet:
//         2. write_monitor_file_wrapper - gaps: need to check if writing into .yuv & .txt are the same.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int write_monitor_files_wrapper(Simulator* sim, output_paths* paths) {

    char* monitor_txt_path = paths->monitor_txt_path;
    char* monitor_yuv_path = paths->monitor_yuv_path;

    // Open the output files for writing
    FILE* monitor_txt_file = checked_fopen(monitor_txt_path, "w");
    if (monitor_txt_file == NULL) {
        printf("Error opening file %s for writing\n", monitor_txt_path);
        return -1; // Return error code
    }

    FILE* monitor_yuv_file = checked_fopen(monitor_yuv_path, "wb");
    if (monitor_yuv_file == NULL) {
        printf("Error opening file %s for writing\n", monitor_yuv_path);
        fclose(monitor_txt_file);
        return -1; // Return error code
    }

    // Extract the display7seg_string from the simulator.
    uint8_t** monitor = sim->monitor;

    //write the monitor string content to the txt file
    if (write_monitor_content_to_file(monitor_txt_file, monitor) != 0) {
        printf("Error writing memory content to file %s\n", monitor_txt_path);
        fclose(monitor_txt_file);
        return -1; // Return error code
    }

    //TODO: need to check if writing into monitor yuv and monitor txt is the same or not.
    //write the monitor content to the YUV file using the appropriate function for 2D arrays
    if (write_monitor_content_to_file(monitor_yuv_file, monitor) != 0) {
        printf("Error writing monitor content to file %s\n", monitor_yuv_path);
        fclose(monitor_yuv_file);
        return -1; // Return error code
    }

    fclose(monitor_txt_file);
    fclose(monitor_yuv_file);
    return 0; // Return success code

}
