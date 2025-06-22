// Defines and includes
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "register.h"
#include "sim_helpers.h"

/////////////////////////////////////////// [TODO] /////////////////////////////////////////
// Todos:
//		
//		* check output paths initialization didn't initialize any of the paths to NULL. (chaeck if i can assume correctness)
//
//
/////////////////////////////////////////// [TODOS] /////////////////////////////////////////
void init_input_paths(input_paths* paths, char* argv[]) {
	paths->memin_path = argv[1];
	paths->diskin_path = argv[2];
	paths->irq2in_path = argv[3];
}

void init_output_paths(output_paths* paths, char* argv[]) {
	// Initialize all paths to NULL or empty strings
	paths->memout_path = argv[4];
	paths->regout_path = argv[5];
	paths->trace_path = argv[6];
	paths->hwregtrace_path = argv[7];
	paths->cycles_path = argv[8];
	paths->leds_path = argv[9];
	paths->display7seg_path = argv[10];
	paths->diskout_path = argv[11];
	paths->monitor_txt_path = argv[12];
	paths->monitor_yuv_path = argv[13];
}

