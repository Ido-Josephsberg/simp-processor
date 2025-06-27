#pragma once
#include <stdio.h>
#include "files_handler.h"
#include "simulator.h"

#define CYCLE_NUM_OF_DIGITS 8
#define PC_NUM_OF_DIGITS 3
#define INST_NUM_OF_DIGITS 8
#define MACRO_TO_STR(x) STR(x) // Interpret the macro and convert it to a string literal
#define STR(x) #x // Converts x to a string literal


// temp function declerations - those functions need to move to thier own file.
int write_memout_file_wrapper(Simulator* sim, output_paths* paths);
int write_trace_file_wrapper(Simulator* sim, output_paths* paths);
int write_hwregtrace_file_wrapper(Simulator* sim, output_paths* paths);
int write_cycles_file_wrapper(Simulator* sim, output_paths* paths);
int write_leds_file_wrapper(Simulator* sim, output_paths* paths);
int write_display7seg_file_wrapper(Simulator* sim, output_paths* paths);
int write_diskout_file_wrapper(Simulator* sim, output_paths* paths);
int write_monitor_files_wrapper(Simulator* sim, output_paths* paths);