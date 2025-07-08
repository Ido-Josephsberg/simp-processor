#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "line_handler.h"

int parse_number(const char* str, int32_t* out_num) {
	/*
	 Parse a number from a string and store it in out_num.
	 The string can be in decimal or hexadecimal format (e.g., "42" or "0x2A").
	 @param: str - a pointer to the string containing the number
	 @param: out_num - a pointer to an int32_t variable to store the parsed number
	 @return: int - return 0 if successful, -1 if error occurred
	 */
	if (!str || !out_num) return -1;

	// Skip leading whitespace, tabs, and newlines
	while (*str == ' ' || *str == '\t' || *str == '\n') {
		str++;
	}

	// Check for empty string after whitespace
	if (*str == '\0') return -1;

	char* endptr = NULL;
	long value = 0;

	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
		value = strtoul(str, &endptr, 16);
	}
	else {
		value = strtoul(str, &endptr, 10);
	}

	// Check if conversion succeeded and no invalid trailing characters (except whitespace)
	if (endptr == str) return -1;
	while (*endptr == ' ' || *endptr == '\t' || *endptr == '\n') endptr++;
	if (*endptr != '\0') return -1;

	*out_num = (int32_t)value;
	return 0;
}

static int init_word_struct(Word* word, char* addr_data_str) {
	/*
	 Initialize a Word struct from a string containing address and data.
	 @param: word - pointer to the Word struct to fill
	 @param: addr_data_str - string containing address and data (e.g., "0x1000 42")
	 @return: int - return 0 if successful, -1 if error occurred
	 */
    if (word == NULL || addr_data_str == NULL) {
        return -1; // Error: Invalid input
    }

    // Extract address and data as separate strings
    char address_str[32], data_str[32];
    int matched = sscanf(addr_data_str, "%31s %31s", address_str, data_str);
    if (matched != 2) {
        printf("Error (line_handler): .word instruction requires two arguments (address and data): %s\n", addr_data_str);
        return -1;
    }

    // Parse address
	uint32_t address;
	 if (parse_number(address_str,&address) != 0) {
        printf("Error (line_handler): Invalid address in.word instruction : % s\n", address_str);
        return -1;
    }

    // Parse data
    int32_t data;
    if (parse_number(data_str, &data) != 0) {//(data == 0 && data_str[0] != '0') {
        printf("Error (line_handler): Invalid data in .word instruction: %s\n", data_str);
        return -1;
    }

	// Initialize the Word struct
    word->address = address;
    word->data = data;
    return 0;
}

static int get_register_code(const char* reg_str) {
    /*
     Convert a register string to its corresponding reg_code enum value.
     @param: reg_str - a pointer to the register string (e.g., "$zero", "$a0", etc.)
     @return: int - the register code (enum value), or -1 if the register is invalid
     */
    if (reg_str == NULL) {
        return -1; // Invalid input
    }

    if (strcmp(reg_str, "$zero") == 0) return zero;
    else if (strcmp(reg_str, "$imm") == 0) return imm;
    else if (strcmp(reg_str, "$v0") == 0) return v0;
    else if (strcmp(reg_str, "$a0") == 0) return a0;
    else if (strcmp(reg_str, "$a1") == 0) return a1;
    else if (strcmp(reg_str, "$a2") == 0) return a2;
    else if (strcmp(reg_str, "$a3") == 0) return a3;
    else if (strcmp(reg_str, "$t0") == 0) return t0;
    else if (strcmp(reg_str, "$t1") == 0) return t1;
    else if (strcmp(reg_str, "$t2") == 0) return t2;
    else if (strcmp(reg_str, "$s0") == 0) return s0;
    else if (strcmp(reg_str, "$s1") == 0) return s1;
    else if (strcmp(reg_str, "$s2") == 0) return s2;
    else if (strcmp(reg_str, "$gp") == 0) return gp;
    else if (strcmp(reg_str, "$sp") == 0) return sp;
    else if (strcmp(reg_str, "$ra") == 0) return ra;

    return -1; // Invalid register code
}

static int get_opcode_code(const char* opcode_str) {
	/*
	 Convert an opcode string to its corresponding opcode enum value.
	 @param: opcode_str - a pointer to the opcode string (e.g., "add", "sub", etc.)
	 @return: int - the opcode enum value, or -1 if the opcode is invalid
	 */
	if (opcode_str == NULL) {
		return -1; // Invalid input
	}

	if (strcmp(opcode_str, "add") == 0) return add;
	else if (strcmp(opcode_str, "sub") == 0) return sub;
	else if (strcmp(opcode_str, "mul") == 0) return mul;
	else if (strcmp(opcode_str, "and") == 0) return and;
	else if (strcmp(opcode_str, "or") == 0) return or ;
	else if (strcmp(opcode_str, "xor") == 0) return xor;
	else if (strcmp(opcode_str, "sll") == 0) return sll;
	else if (strcmp(opcode_str, "sra") == 0) return sra;
	else if (strcmp(opcode_str, "srl") == 0) return srl;
	else if (strcmp(opcode_str, "beq") == 0) return beq;
	else if (strcmp(opcode_str, "bne") == 0) return bne;
	else if (strcmp(opcode_str, "blt") == 0) return blt;
	else if (strcmp(opcode_str, "bgt") == 0) return bgt;
	else if (strcmp(opcode_str, "ble") == 0) return ble;
	else if (strcmp(opcode_str, "bge") == 0) return bge;
	else if (strcmp(opcode_str, "jal") == 0) return jal;
	else if (strcmp(opcode_str, "lw") == 0) return lw;
	else if (strcmp(opcode_str, "sw") == 0) return sw;
	else if (strcmp(opcode_str, "reti") == 0) return reti;
	else if (strcmp(opcode_str, "in") == 0) return in;
	else if (strcmp(opcode_str, "out") == 0) return out;
	else if (strcmp(opcode_str, "halt") == 0) return halt;

	return -1; // Invalid opcode
}

static int init_opcode_and_registers(char* asm_str, Line* line) {
	/*
	 Extract opcode and register codes from an assembly string.
	 @param: asm_str - assembly string (e.g., "add $a0, $a1, $a2")
	 @param: line - pointer to Line struct to fill
	 @return: 0 if successful, -1 if error
	 */
 
	if (asm_str == NULL || line == NULL) return -1;

	char* token = strtok(asm_str, " ,\t");
	if (!token) return -1;
	int opcode = get_opcode_code(token);
	if (opcode < 0) return -1;
	line->opcode = opcode;

	// rd
	token = strtok(NULL, " ,\t");
	if (!token) return -1;
	int rd = get_register_code(token);
	if (rd < 0) return -1;
	line->rd = rd;

	// rs
	token = strtok(NULL, " ,\t");
	if (!token) return -1;
	int rs = get_register_code(token);
	if (rs < 0) return -1;
	line->rs = rs;

	// rt
	token = strtok(NULL, " ,\t");
	if (!token) return -1;
	int rt = get_register_code(token);
	if (rt < 0) return -1;
	line->rt = rt;

	return 0;
}

int parse_instruction_to_line(char* asm_str, Line* line, LineType type) {
	/*
	 Parser assemly row (string) to a Line struct.

	 @param: asm_row - a pointer to a asm string clean from leading whitespces and comments
	 @param: p_line - a pointer to a Line struct to fill with parsed data

	 @return: int - return 0 if successful, -1 if error occurred
	 */
	char asm_str_cpy[MAX_ASM_LINE_LENGTH];
	strncpy(asm_str_cpy, asm_str, MAX_ASM_LINE_LENGTH - 1);
	
	if (line == NULL || asm_str == NULL) {
		printf("Error: Invalid input to parse_instruction_to_line\n");
		return -1; // Error: Invalid input (printed error outside the function)
	}

	// Handle .word line:
	if (type == WORD) {
		char* addr_data_str = asm_str + 5; // Move past ".word"
		while (*addr_data_str == ' ' || *addr_data_str == '\t') {
			addr_data_str++; // Skip leading whitespace
		}

		if (init_word_struct(&(line->word), addr_data_str) != 0) {
			printf("Error (line_handler): failed initializing .word struct from: %s\n", addr_data_str);
			return -1; // Error occurred while initializing word struct
		}
		return WORD; // Successfully parsed .word instruction
	}

	// Handle label callerlabel caller line:
	if (type == Label_call) {
		// Initialize opcode and registers
		if (init_opcode_and_registers(asm_str_cpy, line) < 0) {
			printf("Error: Invalid label caller instruction format: %s\n", asm_str);
			return -1; // Error: Invalid label caller instruction format
		}

		// Initialize reserved bits to zero
		line->reserved = 0;

		// Initialize big imm field to 1
		line->bigimm = 1;

		// Initialize imm8 to zero - its not being used in label caller line
		line->imm8 = 0;

		// Initialize imm32 to zero - the address of the label will be set in the second following line
		line->imm32 = 0;

		// Initialize label name from the asm_str to line->called_label
        char* last_comma = strrchr(asm_str, ',');
        if (last_comma != NULL) {
            char* label_name_start = last_comma + 1; // Move past the comma
            while (*label_name_start == ' ' || *label_name_start == '\t') {
                label_name_start++; // Skip whitespace
            }
			// Find the end of the label name - will help in second pass to insert the correct address to imm32_row
            char* end = label_name_start;
            while (*end != ' ' && *end != '\t' && *end != '\0' && *end != '\n') {
                end++;
            }
            size_t len = end - label_name_start;
            if (len >= MAX_LABEL_NAME_LENGTH) {
                len = MAX_LABEL_NAME_LENGTH - 1;
            }
            strncpy(line->called_label, label_name_start, len);
            line->called_label[len] = '\0'; // Ensure null-termination
			
			return Label_call; // Successfully parsed label caller line
        }
        else {
			printf("Error (line_handler): Invalid label caller instruction format: %s\n", asm_str);
			return -1; // Error: If there is no comma before the Label string the format is invalid (and we were told to assume correctness)
        }	

	}

	// Handle R-type instruction:
	else if (type == R) {
		// Initialize opcode and registers
		if (init_opcode_and_registers(asm_str_cpy, line) < 0) {
			printf("Error (line_handler: Invalid R instruction format: %s\n", asm_str);
			return -1; // Error: Invalid label caller instruction format
		}

		// Initialize reserved bits to zero
		line->reserved = 0;

		char* last_comma = strrchr(asm_str, ',');
		int32_t imm_num = -1; // Initialize immediate value

		// Check if there is a comma in the instruction string
		if (last_comma != NULL) {
			char* imm_str_ptr = last_comma + 1;
			
			// Extract the immediate value from the string after the last comma
			if (parse_number(imm_str_ptr, &imm_num) != 0) {
				printf("Error (line_handler): Invalid immediate value in R-type instruction: %s\n", imm_str_ptr);
				return -1; // Error: Invalid immediate value
			}
			
			// Check if the immediate value is within the valid range for 8-bit or 32-bit immediate
			if (imm_num >= MIN_8BIT_VALUE && imm_num <= MAX_8BIT_VALUE) {
				// Set imm8 the short immediate value in the line struct 
				line->bigimm = 0; // Set bigimm to 0 for R-type instruction
				line->imm8 = (uint8_t)imm_num;
				line->imm32 = 0;  // Set imm32 to 0 - it is not used in this R-type instruction
				strcpy(line->called_label, R_TYPE_NO_LABEL); // Set called_label to a special value indicating no label is used
			}
			else {
				// Set imm32 the long immediate value in the line struct
				line->bigimm = 1; // Set bigimm to 1 for R-type instruction
				line->imm8 = 0;   // Set imm8 to 0 - it is not used in this R-type instruction
				line->imm32 = (uint32_t)imm_num;
				strcpy(line->called_label, R_TYPE_NO_LABEL); // Set called_label to a special value indicating no label is used
			}		
		}
		return R; // Successfully parsed R-type instruction
	}

	printf("Error (line_handler): Unsupported instruction format: %s\n", asm_str);
	return -1; // Error: Unsupported instruction format
}

