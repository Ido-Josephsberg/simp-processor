#pragma once
#include "sim_helpers.h"
#define LEDS_STATUS_NUM_OF_DIGITS 8
#define LEDS_LINE_SIZE (CYCLE_NUM_OF_DIGITS + 1 + LEDS_STATUS_NUM_OF_DIGITS + 1 + 1) // cycle + ' ' + status + '\n' + '\0'