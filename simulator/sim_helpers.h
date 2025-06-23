#include <stdio.h>
#include "files_handler.h"
#include "simulator.h"

// temp function declerations - those functions need to move to thier own file.
int write_memout_file_wrapper(Simulator* sim, output_paths* paths);
int write_trace_file_wrapper(Simulator* sim, output_paths* paths);
int write_hwregtrace_file_wrapper(Simulator* sim, output_paths* paths);
int write_cycles_file_wrapper(Simulator* sim, output_paths* paths);
int write_leds_file_wrapper(Simulator* sim, output_paths* paths);
int write_display7seg_file_wrapper(Simulator* sim, output_paths* paths);
int write_diskout_file_wrapper(Simulator* sim, output_paths* paths);