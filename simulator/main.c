#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "isa_func.h"
#include "simulator.h"
#include "files_handler.h"


/////////////////////////////////////////// [TODO] /////////////////////////////////////////
// Todos:
//		
//		* Continue implementing the main function according to the block diagram.
//
//
/////////////////////////////////////////// [TODOS] /////////////////////////////////////////

int main(int argc, char** argv) {
	// TODO: check if assuming input correctness is allowed.
	// Check if the correct number of arguments is provided
	if (argc < 14) {
		printf("Usage: %s <memin_path> <diskin_path> <irq2in_path> <memout_path> <regout_path> <trace_path> <hwregtrace_path> <cycles_path> <leds_path> <display7seg_path> <diskout_path> <monitor_txt_path> <monitor_yuv_path>\n", argv[0]);
		return -1;
	}
	// Initialize input and output paths
	input_paths in_paths;
	output_paths out_paths;

	init_input_paths(&in_paths, argv);
	init_output_paths(&out_paths, argv);

	//Initialize the simulator fileds
	Simulator sim;
	
	// Allocate memory for the simulator
	
	// Comtinue with the main func addording to the block diagram. 

	return 0;
}