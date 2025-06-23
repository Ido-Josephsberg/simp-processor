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
// 
//      * Unfinished functions and TODOs:
//          1. write_trace_file_wrapper - Implement the logic to extract the trace string from the simulator
//          2. write_hwregtrace_file_wrapper - Implement the logic to extract the hwregtrace string from the simulator.
//          3. write_cycles_file_wrapper - Implement the logic to extract the cycles counter from the simulator
//          4. write_leds_file_wrapper - Implement the logic to extract the leds string from the simulator.
//          5. write_display7seg_file_wrapper - Implement the logic to extract the display7seg string from the simulator.
//  	    6. write_diskout_file_wrapper - Implement the logic to extract the diskout 2d array from the simulator.
//      * functions that are not implemented yet:
// //       7. write_monitor_file_wrapper - gaps: need to check if writing into .yuv & .txt are the same.
//
//
/////////////////////////////////////////// [TODOS] ////////////////////////////////////////////////////////////////////////////////


///////////////* Function to move to the relevent c file *////////////////////////////////////////

//  1. Write Memory to file
int write_memout_file_wrapper(Simulator* sim, output_paths* paths) {
    char* memout_path = paths->memout_path;

    // Open the output file for writing
    FILE* memout_file = checked_fopen(memout_path, "w");
    if (memout_file == NULL) {
        printf("Error opening file %s for writing\n", memout_path);
        return -1; // Return error code
    }

    //write the memory content to the file
    if (write_memory_content_to_file(memout_file, sim->memory) != 0) {
        printf("Error writing memory content to file %s\n", memout_path);
        fclose(memout_file);
        return -1; // Return error code
    }

    // Close the file
    fclose(memout_file);
    return 0; // Return success code
}

//  2. Write Registers to file
// This function is already implemented in register.c as write_regout_file_wrapper - where it should be implemented.

//  3. Write Trace to file
int write_trace_file_wrapper(Simulator* sim, output_paths* paths) {
    char* trace_path = paths->trace_path;
    // Open the output file for writing
    FILE* trace_file = checked_fopen(trace_path, "w");
    if (trace_file == NULL) {
        printf("Error opening file %s for writing\n", trace_path);
        return -1; // Return error code
    }
//*TODO: Implement the logic to extract the trace string from the simulator.
	char* trace_string = "### Example trace string_need to implement real one"; // Placeholder for the actual string extraction logic
    
    //write the trace string content to the file
    if (write_str_to_file(trace_file, trace_string) != 0) {
        printf("Error writing memory content to file %s\n", trace_path);
        fclose(trace_file);
        return -1; // Return error code
    }

	fclose(trace_file);
    return 0;
}

//  4. Write hwregtrace to file
int write_hwregtrace_file_wrapper(Simulator * sim, output_paths * paths) {
    
    char* hwregtrace_path = paths->hwregtrace_path;
    
    // Open the output file for writing
    FILE* hwregtrace_file = checked_fopen(hwregtrace_path, "w");
    if (hwregtrace_file == NULL) {
        printf("Error opening file %s for writing\n", hwregtrace_path);
        return -1; // Return error code
    }
    //*TODO: Implement the logic to extract the hwregtrace string from the simulator.
    char* hwregtrace_string = "### Example hwregtrace string_need to implement real one"; // Placeholder for the actual string extraction logic

    //write the trace string content to the file
    if (write_str_to_file(hwregtrace_file, hwregtrace_string) != 0) {
        printf("Error writing memory content to file %s\n", hwregtrace_path);
        fclose(hwregtrace_file);
        return -1; // Return error code
    }

	fclose(hwregtrace_file);
    return 0;
}

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
int write_leds_file_wrapper(Simulator * sim, output_paths * paths) {
    
    char* leds_path = paths->leds_path;
    
    // Open the output file for writing
    FILE* leds_file = checked_fopen(leds_path, "w");
    if (leds_file == NULL) {
        printf("Error opening file %s for writing\n", leds_path);
        return -1; // Return error code
    }
    //*TODO: Implement the logic to extract the leds string from the simulator.
    char* leds_string = "### Example leds string_need to implement real one"; // Placeholder for the actual string extraction logic

    //write the trace string content to the file
    if (write_str_to_file(leds_file, leds_string) != 0) {
        printf("Error writing memory content to file %s\n", leds_path);
        fclose(leds_file);
        return -1; // Return error code
    }

	fclose(leds_file);
    return 0;
}

//  7. Write display7seg to file
int write_display7seg_file_wrapper(Simulator* sim, output_paths* paths) {

    char* display7seg_path = paths->display7seg_path;

    // Open the output file for writing
    FILE* display7seg_file = checked_fopen(display7seg_path, "w");
    if (display7seg_file == NULL) {
        printf("Error opening file %s for writing\n", display7seg_path);
        return -1; // Return error code
    }
    //*TODO: Implement the logic to extract the display7seg_string from the simulator.
    char* display7seg_string = "### Example leds string_need to implement real one"; // Placeholder for the actual string extraction logic

    //write the trace string content to the file
    if (write_str_to_file(display7seg_file, display7seg_string) != 0) {
        printf("Error writing memory content to file %s\n", display7seg_path);
        fclose(display7seg_file);
        return -1; // Return error code
    }

    fclose(display7seg_file);
    return 0;

}

// 8. Write diskout to file  
int write_diskout_file_wrapper(Simulator* sim, output_paths* paths) {

    char* diskout_path = paths->display7seg_path;

    // Open the output file for writing
    FILE* diskout_file = checked_fopen(diskout_path, "w");
    if (diskout_file == NULL) {
        printf("Error opening file %s for writing\n", diskout_path);
        return -1; // Return error code
    }
    //* TODO: Implement the logic to extract the display7seg_string from the simulator.
    char** diskout_2D_arr = "### Example leds string_need to implement real one"; // Placeholder for the actual string extraction logic

    //write the string content to the file
    if (write_diskout_content_to_file(diskout_file, diskout_2D_arr) != 0) {
        printf("Error writing memory content to file %s\n", diskout_path);
        fclose(diskout_file);
        return -1; // Return error code
    }

    fclose(diskout_file);
    return 0;

}

// 9. Write monitor to file
//TODO: need to check if writing into monitor yuv and monitor txt is the same or not.