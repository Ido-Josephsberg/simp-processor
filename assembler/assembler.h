#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "line_handler.h"

#define MAX_ASM_LINE_LENGTH 500		//check it is enough for the line (or do i need a dynamic buffer?)
#define MAX_MEM_LINES 4069			// memory depth is max 4096 lines.

////////////////////////// TODO ////////////////////////////////////
//  # If there is ----------------------------> please take a look
////////////////////////////////////////////////////////////////////

// Function Declarations
static bool is_label_instruction_inline(char* colon_pos);
static void update_label_address_dict(Label* label_dict_arg, char* label_name, uint32_t label_address);
static bool is_label_caller_line(char* asm_str);
static bool is_R_line(char* asm_str);
static int set_Imm32_line_struct(Line* line, char* label_name); static uint32_t R_line_to_hex(const Line* line);
static uint32_t Imm32_line_to_hex(const Line* line);

int first_pass(FILE* asm_program, Line* line_arr, Label* label_dict, int* asm_line_count, int* label_count);
int second_pass(Line* line_arr, int line_count, Label* label_dict, int label_count, uint32_t* memin, unsigned int* word_is_zero_location);

// Assembler Related Functions
// TODO # TODO # BOM

#endif // ASSEMBLER_H