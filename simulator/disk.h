#pragma once
#include <stdio.h>
#include "simulator.h"


// Forward declaration of the Simulator struct
typedef struct Simulator Simulator;

int write_diskout_file_wrapper(Simulator* sim, output_paths* paths);

