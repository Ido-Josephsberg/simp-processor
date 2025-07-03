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
/////////////////////////////////////////// [TODOS] //////////////////////////////////////////////////////////////////////////////


//////////////////////* Function to move to the relevent c file */////////////////////////////////

//  1. Write Memory to file
        //Moved to memory.c

//  2. Write Registers to file
// This function is already implemented in register.c as write_regout_file_wrapper - where it should be implemented.

//  3. Write Trace to file
        //Moved to trace_handler.c

//  4. Write hwregtrace to file
        //Moved to hwregtrace_handler.c

//  5. Write cycles to file
		// Moved to cycles.c

//  6. Write leds to file
        //Moved to leds_handler.c

//  7. Write display7seg to file
        //Moved to seg7display_handler.c

// 8. Write disk to file  
        //Moved to disk.c
        
// 9. Write monitor to file
		//Moved to monitor.c
