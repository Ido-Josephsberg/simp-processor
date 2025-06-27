#include <stdio.h>
#include <string.h>
#include "dynamic_str.h"


DynamicString* create_dynamic_str() {
	/*
	Create a new dynamic string with an initial size of 4096 bytes.
	*/

	// Initial size of the string buffer.
	const size_t initial_size = 4096;
	// Allocate memory for the DynamicString structure.
	DynamicString* str = (DynamicString*)malloc(sizeof(DynamicString));
	// Check if memory allocation was successful, if not, notify and exit the program with MEMORY_ERROR.
	if (str == NULL) {
		printf("Memory allocation failed\n");
		exit(MEMORY_ERROR);
	}
	// Allocate memory for the string data with the initial size.
	str->data = (char*)malloc(initial_size * sizeof(char));
	// Check if memory allocation for the string data was successful, if not, free the structure, notify
	// and exit the program with MEMORY_ERROR.
	if (str->data == NULL) {
		free(str);
		printf("Memory allocation failed\n");
		exit(MEMORY_ERROR);
	}
	// Set the allocated memory size to the initial size.
	str->alloc_size = initial_size;
	// Return the pointer to the newly created dynamic string structure.
	return str;
}

void dynamic_str_reallocate(DynamicString* str) {
	/*
	realocate the memory block to a new size that is the square of the original size and update the pointer. returns the new size of the memory block.
	exits the program with MEMORY_ERROR if the reallocation fails.
	ptr: Pointer to the pointer that holds the memory address to be reallocated.
	size: The size of the original memory block to be reallocated.
	returns: The new size of the memory block after reallocation.
	*/

	// Increase the size of the allocated memory by squaring his current size. 
	str->alloc_size = str->alloc_size * str->alloc_size;
	// Attempt to reallocate memory
	void* new_ptr = realloc(str->data, str->alloc_size);
	// Check if the reallocation was successful
	if (new_ptr == NULL) {
		// In case of memory allocation failure, print an error message and exit the program.
		printf("Memory reallocation failed\nExit from simulator");
		exit(MEMORY_ERROR);
	}
	// Update the original pointer to point to the newly allocated memory block.
	str->data = new_ptr;
}
void ensure_str_capacity(DynamicString* str, int needed_size_buffer) {

	/*
	Ensure that the string has enough allocated memory to append string with needed_size_buffer size including the null terminator.
	str: Pointer to the string that needs capacity adjustment.
	allocated_memory_size_ptr: Pointer to the variable storing the current allocated memory size.
	needed_size_buffer: The required size the string should be able to hold.
	*/

	// Calculate the total size needed for the new string, which is the current string length plus the size of the buffer needed.
	size_t size_of_new_str = strlen(str->data) + needed_size_buffer;
	// While the current allocated memory size is less than the size needed for the new string, reallocate memory and update the current allocated memory size (to square of the original size).
	while (str->alloc_size < size_of_new_str) {
		dynamic_str_reallocate(str);
	}
}