#pragma once
// General macros for the project
#define CYCLE_NUM_OF_DIGITS 8
#define PC_NUM_OF_DIGITS 3
#define INST_NUM_OF_DIGITS 8
#define MACRO_TO_STR(x) STR(x) // Interpret the macro and convert it to a string literal
#define STR(x) #x // Converts x to a string literal
#define MEMORY_ERROR -1

// Macros related to Memory
#define MEMORY_SIZE 4096            // Size of memory in words
#define MEMORY_LINE_HEX_DIGITS 8 // Number of hex digits per memory line

// Macros related to disk
#define DISK_LINE_HEX_DIGITS 8		// Number of hex digits in a disk line
#define DISK_SECTORS 128            // Number of sectors in hard disk   
#define DISK_ROWS 128               // Number of rows in hard disk's sector

// Macros related to registers
#define REG_NUM 16			// Number of registers in the CPU
#define REG_STR_SIZE 8		// Size of register string representation

// Macros related to Monitor
#define PIXEL_PER_ROW_COL 256 // Number of pixels per row/column - monitor file
#define MONITOR_DIMENSION 2 // Monitor dimensions - 2D array

// Macros related to file handling
#define FILE_ERROR -2
#define INPUT_ERROR -3
#define MAX_UINT_32 4294967295 // Maximum value for a 32-bit unsigned integer

// Macros related to ISA
#define ISA_FUNC_NUM_OF_FUNCS 22

// Macros related to IO registers
#define IO_REG_NUM 23
#define IO_REG_MAX_NAME_SIZE 12

// Macros related to the segment 7 display
#define SEG7DISPLAY_STATUS_NUM_OF_DIGITS 8
#define SEG7DISPLAY_LINE_SIZE (CYCLE_NUM_OF_DIGITS + 1 + SEG7DISPLAY_STATUS_NUM_OF_DIGITS + 1 + 1) // cycle + ' ' + status + '\n' + '\0'

// Macros related to the trace file
#define TRACE_LINE_SIZE 167 // 165 + newline + null terminator

// Macros related to the LED display
#define LEDS_STATUS_NUM_OF_DIGITS 8
#define LEDS_LINE_SIZE (CYCLE_NUM_OF_DIGITS + 1 + LEDS_STATUS_NUM_OF_DIGITS + 1 + 1) // cycle + ' ' + status + '\n' + '\0'

// Macros related to the io register trace
#define HWREGTRACE_DATA_NUM_OF_DIGITS 8
#define HWREGTRACE_LINE_SIZE (CYCLE_NUM_OF_DIGITS + 1 + 5 + 1 + IO_REG_MAX_NAME_SIZE + 1 + HWREGTRACE_DATA_NUM_OF_DIGITS + 2) // Length of line in hardware register trace - including newline and null terminator

// Macros related to Fetch and Decode
#define IMM_NUM_OF_BYTES 8 
#define STACK_OVERFLOW_ERROR -4 // Error code for stack overflow