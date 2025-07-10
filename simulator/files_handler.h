#ifndef FILES_H // Is it needed?
#define FILES_H // Is it needed?
#include "macros.h"
#include "dynamic_mem.h"


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

} input_paths;

//////////////////////////// Fuctions decleration ////////////////////////////////
void init_input_paths(input_paths* paths, char* argv[]);
void init_output_paths(output_paths* paths, char* argv[]);
uint32_t str_hex_to_uint32(const char* hex_str, int* error);
int read_memin_file(input_paths* paths, uint32_t mem[], int* max_address);
int read_diskin_file(input_paths* paths, uint32_t disk[][DISK_ROWS]);
int read_irq2in_file(input_paths* paths, DynamicMem* irq2in, int* num_of_cycles);

#endif // FILES_H