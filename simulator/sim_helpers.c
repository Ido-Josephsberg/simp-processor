// Defines and includes
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "register.h"
#include "sim_helpers.h"
#include "write_helpers.h"
#include "simulator.h"

/////////////////////////////////////////// [TODO] ////////////////////////////////////////////////////////////////////////////////
// Todos:
//		
//		* check output paths initialization didn't initialize any of the paths to NULL. (chaeck if i can assume correctness)
//      * //* TODO: check with ido if the string data will contain the line drops or not.
// 
//      * Unfinished functions and TODOs:
//          1. write_cycles_file_wrapper - Implement the logic to extract the cycles counter from the simulator
//      * functions that are not implemented yet:
// //       2. write_monitor_file_wrapper - gaps: need to check if writing into .yuv & .txt are the same.
//
//
/////////////////////////////////////////// [TODOS] ////////////////////////////////////////////////////////////////////////////////


///////////////* Function to move to the relevent c file *////////////////////////////////////////

//  1. Write Memory to file
        //Moved to memory.c

//  2. Write Registers to file
// This function is already implemented in register.c as write_regout_file_wrapper - where it should be implemented.

//  3. Write Trace to file
        //Moved to trace_handler.c

//  4. Write hwregtrace to file
        //Moved to hwregtrace_handler.c

//  5. Write cycles to file
int write_cycles_file_wrapper(Simulator* sim, output_paths* paths) {

    char* cycles_path = paths->cycles_path;

    // Open the output file for writing
    FILE* cycles_file = checked_fopen(cycles_path, "w");
    if (cycles_file == NULL) {
        printf("Error opening file %s for writing\n", cycles_path);
        return -1; // Return error code
    }
    //*TODO: Implement the logic to extract the cycles number from the simulator.
    // consider if it needs to be int32_t. if int32-t, then change in write_num_to_file as well
    int cycles_num = "### Example num need to implement real one"; // Placeholder for the actual num extraction logic

    //write the trace string content to the file
    if (write_num_to_file(cycles_file, cycles_num) != 0) {
        printf("Error writing cycles content to file %s\n", cycles_path);
        fclose(cycles_file);
        return -1; // Return error code
    }

    fclose(cycles_file);
    return 0;
}

//  6. Write leds to file
        //Moved to leds_handler.c

//  7. Write display7seg to file
        //Moved to seg7display_handler.c

// 8. Write disk to file  
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

// 9. Write monitor to file
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
	//write the monitor string content to the YUV file (since monitor is a string, we will write it the same for a binary file)
    if (write_str_to_file(monitor_yuv_file, monitor) != 0) {
        printf("Error writing memory content to file %s\n", monitor_yuv_path);
        fclose(monitor_yuv_file);
        return -1; // Return error code
    }

	fclose(monitor_txt_file);
    fclose(monitor_yuv_file);
	return 0; // Return success code

}
