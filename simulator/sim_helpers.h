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
