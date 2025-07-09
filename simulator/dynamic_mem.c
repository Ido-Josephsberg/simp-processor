#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "dynamic_mem.h"


DynamicMem* create_dynamic_mem() {
	/*
	Create a new dynamic memory with an initial size of 4096 bytes.
	*/

	// Initial size of the memory.
	const size_t initial_size = 4096;
	// Allocate memory for the DynamicMem structure.
	DynamicMem* mem_ptr = (DynamicMem*)malloc(sizeof(DynamicMem));
	// Check if memory allocation was successful, if not, notify and exit the program with MEMORY_ERROR.
	if (mem_ptr == NULL) {
		printf("Memory allocation failed\n");
		exit(MEMORY_ERROR);
	}
	// Allocate memory for the data with the initial size.
	mem_ptr->data = calloc(initial_size,1);
	// Check if memory allocation for the data was successful, if not, free the structure, notify
	// and exit the program with MEMORY_ERROR.
	if (mem_ptr->data == NULL) {
		free(mem_ptr);
		printf("Memory allocation failed\n");
		exit(MEMORY_ERROR);
	}
	// Set the allocated memory size to the initial size.
	mem_ptr->alloc_size = initial_size;
	// Return the pointer to the newly created dynamic memory structure.
	return mem_ptr;
}

void dynamic_mem_reallocate(DynamicMem* mem) {
	/*
	realocate the memory block to a new size that is the double of the original size and update the pointer.
	exits the program with MEMORY_ERROR if the reallocation fails.
	mem: Pointer to the dynamic memory that needs reallocation.
	*/

	// Increase the size of the allocated memory by doubling its current size. 
	mem->alloc_size *= 2;
	// Attempt to reallocate memory
	void* new_ptr = realloc(mem->data, mem->alloc_size);
	// Check if the reallocation was successful
	if (new_ptr == NULL) {
		// In case of memory allocation failure, print an error message and exit the program.
		printf("Memory reallocation failed\nExit from simulator");
		exit(MEMORY_ERROR);
	}
	// Update the original pointer to point to the newly allocated memory block.
	mem->data = new_ptr;
}

void ensure_str_capacity(DynamicMem* str, size_t needed_size_buffer) {
	/*
	Ensure that the string has enough allocated memory to append string with needed_size_buffer size including the null terminator.
	str: Pointer to the string that needs capacity adjustment.
	allocated_memory_size_ptr: Pointer to the variable storing the current allocated memory size.
	needed_size_buffer: The required size the string should be able to hold.
	*/

	// Calculate the total size needed for the new string, which is the current string length plus the size of the buffer needed.
	size_t size_of_new_str = strlen((char*)str->data) + needed_size_buffer;
	// While the current allocated memory size is less than the size needed for the new string, reallocate memory and update the current allocated memory size (to double of the original size).
	while (str->alloc_size < size_of_new_str) {
		dynamic_mem_reallocate(str);
	}
}
