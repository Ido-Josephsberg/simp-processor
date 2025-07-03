#pragma once
#include "sim_helpers.h"
#define SEG7DISPLAY_STATUS_NUM_OF_DIGITS 8
#define SEG7DISPLAY_LINE_SIZE (CYCLE_NUM_OF_DIGITS + 1 + SEG7DISPLAY_STATUS_NUM_OF_DIGITS + 1 + 1) // cycle + ' ' + status + '\n' + '\0'

void update_7seg(Simulator* sim);
int write_display7seg_file_wrapper(Simulator* sim, output_paths* paths);