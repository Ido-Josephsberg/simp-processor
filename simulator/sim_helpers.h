#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////// structs ////////////////////////////////
typedef struct output_paths {
	char* memout_path;
	char* regout_path;
	char* trace_path;
	char* hwregtrace_path;
	char* cycles_path;
	char* leds_path;
	char* display7seg_path;
	char* diskout_path;
	char* monitor_txt_path;
	char* monitor_yuv_path;

} output_paths;

typedef struct input_paths {
	char* memin_path;
	char* diskin_path;
	char* irq2in_path;
	char* monitor_yuv_path;

} input_paths;

//////////////////////////// Fuctions decleration ////////////////////////////////
void init_input_paths(input_paths* paths, char* argv[]);
	
void init_output_paths(output_paths* paths, char* argv[]);