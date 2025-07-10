#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "line_handler.h"

// Function Declarations
int first_pass(FILE* asm_program, Line* line_arr, Label* label_dict, int* asm_line_count, int* label_count);
int second_pass(Line* line_arr, int line_count, Label* label_dict, int label_count, uint32_t* memin, unsigned int* word_is_zero_location);

#endif // ASSEMBLER_H