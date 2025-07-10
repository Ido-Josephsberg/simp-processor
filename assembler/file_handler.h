#pragma once

#include <stdio.h>

// Function declerations
FILE* checked_fopen(const char* path, const char* mode);
int find_last_nonzero_location(uint32_t* memin, unsigned int* word_is_zero_locations);
int write_memin_to_file(FILE* file, uint32_t* memin, size_t memin_len);