#pragma once
#include <stdlib.h>
#define MEMORY_ERROR 1

// Dynamic string structure
// This structure is used to manage a dynamically allocated string.
// For example, a string of output file, which can change size during the program execution.
typedef struct {
	char* data;			// Pointer to the string data
	size_t alloc_size; // Allocated size in characters
} DynamicString;

DynamicString* create_dynamic_str();
void dynamic_str_reallocate(DynamicString* str);
void ensure_str_capacity(DynamicString* str, int needed_size_buffer);

