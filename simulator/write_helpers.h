#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////// Write Functions ////////////////////////////////
FILE* checked_fopen(const char* path, const char* mode);

int write_str_to_file(FILE* file, char* str);

int write_num_to_file(FILE* file, int num);

int write_registers_content_to_file(FILE* file, int32_t* reg_array);

int write_memory_content_to_file(FILE* file, uint32_t* memory);

int write_disk_content_to_file(FILE* file, int32_t** disk);

int write_monitor_content_to_file(FILE* file, uint8_t** monitor);