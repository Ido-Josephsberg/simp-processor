#pragma once
#define HWREGTRACE_DATA_NUM_OF_DIGITS 8
#define HWREGTRACE_LINE_SIZE (CYCLE_NUM_OF_DIGITS + 1 + 5 + 1 + IO_REG_MAX_NAME_SIZE + 1 + HWREGTRACE_DATA_NUM_OF_DIGITS + 2) // Length of line in hardware register trace - including newline and null terminator




void update_hwregtrace(Simulator* sim);
