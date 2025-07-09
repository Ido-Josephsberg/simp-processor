#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "isa_func.h"
#include "simulator.h"
#include "files_handler.h"
#define True 1
#define False 0
#include "io_register.h"
#include "monitor.h"
#include "fetch_n_decode.h"
#include "trace_handler.h"
#include "hwregtrace_handler.h"
#include "leds_handler.h"
#include "seg7display_handler.h"
#include "cycles.h"
#include "disk.h"
#include "monitor.h"




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

	// Initialize the simulator object.
	Simulator sim;
	simulator_init(&sim, in_paths, out_paths);
	fetch_n_decode_loop(&sim);
	write_trace_file_wrapper(&sim, &out_paths);
	write_hwregtrace_file_wrapper(&sim, &out_paths);
	write_cycles_file_wrapper(&sim, &out_paths);
	write_leds_file_wrapper(&sim, &out_paths);
	write_display7seg_file_wrapper(&sim, &out_paths);
	write_diskout_file_wrapper(&sim, &out_paths);
	write_monitor_files_wrapper(&sim, &out_paths, sim.max_monitor_pixel);
	write_memout_file_wrapper(&sim, &out_paths);
	write_regout_file_wrapper(&sim, &out_paths);

	return 0;
}