#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "line_handler.h"

#define MIN_8BIT_VALUE -128
#define MAX_8BIT_VALUE 127

///////////////////////////////////// TODO ///////////////////////////////////////////////
//	1. Solved issues / questions in lines marked with "TODO" or ---------------------->	
// 
//	questions:
// 			1. word.data - can be negative? -- FOR NOW I ASSUME IT CAN BE NEGATIVE
//
//////////////////////////////////////////////////////////////////////////////////////////

int parse_number(const char* str) {
	// Note: if strlol() fails to convert the string, it returns 0.
	// This uniqe case is being checked outside this function - after it is called.

	if (str != NULL && (str[0] == '0') && (str[1] == 'x' || str[1] == 'X')) {
		// Hexadecimal
		return strtol(str, NULL, 16);
	}
	else {
		// Always decimal, even if leading zero
		return strtol(str, NULL, 10);
	}
}

static int init_word_struct(Word* word, char* addr_data_str) {
    if (word == NULL || addr_data_str == NULL) {
        return -1; // Error: Invalid input
    }

    // Skip leading whitespace
    while (*addr_data_str == ' ' || *addr_data_str == '\t') {
        addr_data_str++;
    }

    // Extract address and data as separate strings
    char address_str[32], data_str[32];
    int matched = sscanf(addr_data_str, "%31s %31s", address_str, data_str);
    if (matched != 2) {
        printf("Error (line_handler): .word instruction requires two arguments (address and data): %s\n", addr_data_str);
        return -1;
    }

    // Parse address
    uint32_t address = parse_number(address_str);
    if (address == 0 && address_str[0] != '0') {
        printf("Error (line_handler): Invalid address in.word instruction : % s\n", address_str);
        return -1;
    }

    // Parse data
    int32_t data = parse_number(data_str); //------------------------------------------------------------> CHECK IF DATA CAN BE NEGATIVE
    if (data == 0 && data_str[0] != '0') {
        printf("Error (line_handler): Invalid data in .word instruction: %s\n", data_str);
        return -1;
    }

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
		if (init_opcode_and_registers(asm_str, line) < 0) {
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
		if (init_opcode_and_registers(asm_str, line) < 0) {
			printf("Error (line_handler: Invalid R instruction format: %s\n", asm_str);
			return -1; // Error: Invalid label caller instruction format
		}

		// Initialize reserved bits to zero
		line->reserved = 0;

		char* last_comma = strrchr(asm_str, ',');
		int imm = 0; // Initialize immediate value

		if (last_comma != NULL) {
			char* imm_arg = last_comma + 1;

			while (*imm_arg == ' ' || *imm_arg == '\t') {
				imm_arg++; // Skip whitespace
			}
			imm = parse_number(*imm_arg);

			// Check if the immediate value is zero or its representation is invalid from strlot failure
			if (imm == 0) {
				int i = 0;		// Index for skipping leading zeros
				if (imm_arg[1] == 'x' || imm_arg[1] == 'X') {
					// Hex: skip leading zeros
					i = 2; // Skip "0x" or "0X"
				}
				// Decimal: skip leading zeros
				while (*(imm_arg + i) == '0') imm_arg++;

				// NOT(If nothing left, or only whitespace, it's zero)
				if (!(*imm_arg == '\0' || isspace((unsigned char)*imm_arg))) {
					printf("Error: Invalid immediate value in R-type instruction: %s\n", imm_arg);
					return -1; // Error: Invalid immediate value
				}
			}
			if (imm >= MIN_8BIT_VALUE && imm <= MAX_8BIT_VALUE) {
				// Set imm8 the short immediate value in the line struct 
				line->bigimm = 0; // Set bigimm to 0 for R-type instruction
				line->imm8 = (uint8_t)imm;
				line->imm32 = 0;  // Set imm32 to 0 - it is not used in this R-type instruction
				strcpy(line->called_label, R_TYPE_NO_LABEL); // Set called_label to a special value indicating no label is used
			}
			else {
				// Set imm32 the long immediate value in the line struct
				line->bigimm = 1; // Set bigimm to 1 for R-type instruction
				line->imm8 = 0;   // Set imm8 to 0 - it is not used in this R-type instruction
				line->imm32 = (uint32_t)imm;
				strcpy(line->called_label, R_TYPE_NO_LABEL); // Set called_label to a special value indicating no label is used
			}
			
		}

		return R; // Successfully parsed R-type instruction
	}

	printf("Error (line_handler): Unsupported instruction format: %s\n", asm_str);
	return -1; // Error: Unsupported instruction format
}

