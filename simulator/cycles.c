#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "cycles.h"

/////////////////////////////////////////// [TODO] ////////////////////////////////////////////////////////////////////////////////
// Todos:
// 		* implement the cycles counter logic here
// 
//      * Unfinished functions and TODOs:
//          1. write_cycles_file_wrapper - Implement the logic to extract the cycles counter from the simulator
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

    //write the cycles content to the file
    if (write_num_to_file(cycles_file, cycles_num) != 0) {
        printf("Error writing cycles content to file %s\n", cycles_path);
        fclose(cycles_file);
        return -1; // Return error code
    }

    fclose(cycles_file);
    return 0;
}