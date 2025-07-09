#pragma once

#include <stdio.h>
#include "simulator.h"

void init_monitor(Simulator* sim);
int write_monitor_files_wrapper(Simulator* sim, output_paths* paths, int max_monitor_pixel[]);
