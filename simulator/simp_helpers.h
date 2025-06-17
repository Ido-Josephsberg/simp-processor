#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////// Write Functions ////////////////////////////////
FILE* checked_fopen(const char* path, const char* mode);

int write_str_to_file(FILE* file, char* str);

int write_num_to_file(FILE* file, int num);

int write_registers_to_file(FILE* file, int32_t* reg_array);

