#pragma once
#include <string.h>
#include "dynamic_mem.h"

// Dynamic string structure
// This structure is used to manage a dynamically allocated string.
// For example, a string of output file, which can change size during the program execution.
typedef struct {
	void* data;			// Pointer to the dynamically allocated data
	size_t alloc_size;  // Allocated size in units of bytes
} DynamicMem;

DynamicMem* create_dynamic_mem();
void dynamic_mem_reallocate(DynamicMem* mem);
void ensure_str_capacity(DynamicMem* str, size_t needed_size_buffer);

