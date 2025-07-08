#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "line_handler.h"
#include "assembler.h"
#include "file_handler.h"

static bool is_label_instruction_inline(char* colon_pos) {
	/*
	 Check if the label instruction is inline with an instruction.
	 A label instruction is inline if it has an instruction after the colon.
	 @param: colon_pos - a pointer to the position of the colon in the assembly string
	 @return: bool - true if it is inline, false otherwise
	 */
	if (colon_pos == NULL) {
		return false;
	}
	// Check if there is an instruction after the label
	char* instruction_start = colon_pos + 1;

	// Skip leading whitespace after the colon
	while (*instruction_start == ' ' || *instruction_start == '\t') {
		instruction_start++;
	}
	return (*instruction_start != '\0' && *instruction_start != '\n') ? true : false;
}

static void update_label_address_dict(Label* label_dict_arg,char* label_name, uint32_t label_address) {
	/*
	 Update the label dictionary with the label name and address.
	 @param: label_dict_arg - pointer to the Label struct to update
	 @param: label_name - string representing the label name
	 @param: label_address - address of the label
	 */
	strcpy(label_dict_arg->label_name, label_name);
	label_dict_arg->label_address = label_address;
}

static bool is_label_caller_line(char* asm_str) {
	/*
	 Check if the given assembly string is a label caller line.
	 A label caller line defined as a line that contains label in the imm field, which is the last argument in the instruction.
	 @param: asm_str - a pointer to the assembly string to check
	 @return: bool - true if it is a label caller line, false otherwise
	 */
	if (asm_str == NULL) {
		return false; // Invalid input
	}
	char* last_comma = strrchr(asm_str, ',');
	if (last_comma != NULL) {
		char* imm_arg = last_comma + 1;
		while (*imm_arg == ' ' || *imm_arg == '\t') {
			imm_arg++; // Skip whitespace
		}
		return isalpha(*imm_arg) != 0 ? true : false;
	}
}

static bool is_R_line(char* asm_str) {
	/*
	Check if the given assembly string is an R-type instruction line.
	An R-type instruction line is defined as a line that has a comma before the immediate value (which is an int in R type).
	@param: asm_str - a pointer to the assembly string to check
	@return: bool - true if it is an R-type instruction line, false otherwise
	*/
	if (asm_str == NULL) {
		return false; // Invalid input
	}
	char* last_comma = strrchr(asm_str, ',');
	if (last_comma != NULL) {
		char* imm_str_ptr = last_comma + 1;

		int32_t imm_num = -1; // Initialize immediate value
		if (parse_number(imm_str_ptr, &imm_num) != 0) { // Check if the immediate value is valid and assign it to imm_num
			printf("Error (line_handler): Invalid immediate value in R-type instruction: %s\n", imm_str_ptr);
		}
		return true;
	}
	else
	{
		return false;
	}
}

static int set_Imm32_line_struct(Line* line, char* label_name) {
	/*
	 Set the Line struct for an Imm32 line.
	 @param: line - pointer to the Line struct to fill with parsed data
	 @param: label_name - string representing the label name
	 @return: int - return 0 if successful, -1 if error occurred
	 */
	if (line == NULL || label_name == NULL) {
		return -1; // Error: Invalid input
	}
	line->type = Imm32; // Set the line type to Imm32
	line->imm32 = 0; // Initialize imm32 to zero - the address of the label will be set in the second pass according to the label dictionary
	
	// Copy the label name to the line's called_label field
	strncpy(line->called_label, label_name, MAX_LABEL_NAME_LENGTH - 1);
	line->called_label[MAX_LABEL_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
	return 0; // Success
}

static uint32_t R_line_to_hex(const Line* line) {
	/*
	 Convert an R line to its corresponding hexadecimal representation.
	 @param: line - pointer to the Line struct representing the R line
	 @return: uint32_t - the hexadecimal representation of the R line
	 */
	uint32_t hex = 0;
	hex |= ((line->opcode & 0xFF) << 24);
	hex |= ((line->rd & 0x0F) << 20);
	hex |= ((line->rs & 0x0F) << 16);
	hex |= ((line->rt & 0x0F) << 12);
	hex |= ((line->reserved & 0x07) << 9);
	hex |= ((line->bigimm & 0x01) << 8);
	hex |= (line->imm8 & 0xFF);
	return hex;
}

static uint32_t Imm32_line_to_hex(const Line* line) {
	/*
	 Convert an Imm32 line to its corresponding hexadecimal representation.
	 @param: line - pointer to the Line struct representing the Imm32 line
	 @return: uint32_t - the hexadecimal representation of the Imm32 line
	 */
	return (line->imm32 & 0xFFFFFFFF);
}

int first_pass(FILE* asm_program, Line* line_arr, Label* label_dict, int* asm_line_count, int* label_count) {
    /*
    Perform the first pass of the assembler to parse the assembly file and fill the line array and label dictionary.
    @param asm_program: Pointer to the assembly file opened in read mode.
    @param line_arr: Pointer to the array of Line structs to fill with parsed data.
    @param label_dict: Pointer to the array of Label structs to fill with parsed label addresses.
    @return: line_index+1, label_dict_index+1 - Returns the number of lines processed and the number of labels, or -1 if an error occurred.
    */

	if (asm_program == NULL || line_arr == NULL) {
		return -1; // Error: Invalid file or line array
	}
	
	char asm_row_str[MAX_ASM_LINE_LENGTH];      // Buffer to hold extracted line from the assembly file
	int line_index = 0;						    // Index that is updated according to the current handled asm line
	int label_dict_index = 0;					// Index for the label dictionary
	
	while (fgets(asm_row_str, sizeof(asm_row_str), asm_program) != NULL) { //Reminder: fgets adds a null terminator at the end of the string
		
		// Clean the line from comments and whitespace
		char* comment_pos = strchr(asm_row_str, '#');
		if (comment_pos != NULL) {
			*comment_pos = '\0'; // Remove comment
		}
		
		// Trim leading whitespace
		char* trimmed_asm_str = asm_row_str;
		while (*trimmed_asm_str == ' ' || *trimmed_asm_str == '\t') {
			trimmed_asm_str++;
		}
		
		// Check if the line is empty after trimming
		if (*trimmed_asm_str == '\0') {
			continue; // Skip empty lines
		}

	/*This section handles Label addresses - Finds rows that are the destination of a called Label*/
		char* colon_pos = strchr(trimmed_asm_str, ':');
		// Check asm_row_str is Label instruction line that doesn't have an instruction after it (in the same line e.g. "LOOP:" and nothing more)
		if (colon_pos != NULL) {
			// Extract label_name from asm_str:
			char label_name_str[MAX_LABEL_NAME_LENGTH];
			int len = (int)(colon_pos - trimmed_asm_str);
			if (len >= MAX_LABEL_NAME_LENGTH) len = MAX_LABEL_NAME_LENGTH - 1;
			strncpy(label_name_str, trimmed_asm_str, len);
			label_name_str[len] = '\0';

			// Check if the label is NOT inline with an instruction. If so, it is a label-only line
			if (!(is_label_instruction_inline(colon_pos))) {
				
				// Update the label dictionary array with the label name and address
				update_label_address_dict(&(label_dict[label_dict_index]), label_name_str, line_index);  
				label_dict_index++; // Increment label dictionary index for next label

				//Skip line that is label only (doesn't contain an instruction) - done by not incrementing line_index
				continue;
			}

			else // Label is inline with an instruction
			{
				// Update the label dictionary array with the label name and address
				update_label_address_dict(&(label_dict[label_dict_index]), label_name_str, line_index);
				label_dict_index++; // Increment label dictionary index for next label

				// Update the trimmed_asm_str to start after the label name for being parsed by asm instruction format = opcode $rd, $rs, $rt, imm8
				trimmed_asm_str = colon_pos + 1; // Move past the colon

				// Trim leading whitespace
				while (*trimmed_asm_str == ' ' || *trimmed_asm_str == '\t') {
					trimmed_asm_str++;
				}
			}
		}
	/*end Label dest section*/

	/*This section distinguish different Line Type*/
		// checks if .word line:
		if (strncmp(trimmed_asm_str, ".word", 5) == 0) {
			line_arr[line_index].type = WORD; // Set the line type to WORD

			int line_type = parse_instruction_to_line(trimmed_asm_str, &(line_arr[line_index]),WORD); // returns line type according to LineType enum or -1 if fails
			if (line_type < 0) {
				//Error are habdeled inside the function
				return  -1; // Error parsing instruction
			}
			else if (line_type != WORD) {
				printf("Error: Expected .word instruction, got %d\n", line_type);
				return -1; // Error: Expected .word instruction
			}
			line_index++; // Increment line index for the next line
		}
		
		// Checks if label caller line:
		else if (is_label_caller_line(trimmed_asm_str)) {
			line_arr[line_index].type = Label_call; // Set the line type to Label_call

			int line_type = parse_instruction_to_line(trimmed_asm_str, &(line_arr[line_index]), Label_call); // returns line type according to LineType enum or -1 if fails
			if (line_type < 0) {
				//Error are habdeled inside the function
				return -1; // Error parsing instruction
			}
			else if (line_type != Label_call) {
				printf("Error (assembler): Expected Label_call instruction, got %d\n", line_type);
				return -1; // Error: Expected .word instruction
			}
			// Check if there is enough space in the line array for the next line
			if (line_index + 1 >= MAX_MEM_LINES) {
				printf("Error (assembler): Line index out of bounds, cannot set Imm32 line struct\n");
				return -1;
			}
			// Set the Imm32 line struct for the next line
			if (set_Imm32_line_struct(&(line_arr[line_index + 1]), line_arr[line_index].called_label) < 0) {
				printf("Error (assembler): Failed to set Imm32 line struct\n");
				return -1;
			}
			line_index += 2; // Increment line index for the opcode line (two lines are used for Label_call instruction)
		}

		// Checks if R line:
		else if (is_R_line(trimmed_asm_str)) {
			line_arr[line_index].type = R; // Set the line type to R

			int line_type = parse_instruction_to_line(trimmed_asm_str, &(line_arr[line_index]), R); // returns line type according to LineType enum or -1 if fails
			if (line_type < 0) {
				//Error are habdeled inside the function
				return -1; // Error parsing instruction
			}
			else if (line_type != R) {
				printf("Error (assembler): Expected R instruction, got %d\n", line_type);
				return -1; // Error: Expected .word instruction
			}

			//Check if bigimm is set to 1, if so, the next line is an Imm32 line
			if (line_arr[line_index].bigimm == 1) {
				// Check if there is enough space in the line array for the next line
				if (line_index + 1 >= MAX_MEM_LINES) {
					printf("Error: Line index out of bounds, cannot set Imm32 line struct\n");
					return -1; // Error: Line index out of bounds
				}
				// Set the Imm32 line struct for the next line
				line_arr[line_index + 1].type = Imm32; // Set the line type to Imm32
				line_arr[line_index + 1].imm32 = line_arr[line_index].imm32; // Copy the imm32 value from the R line
				strncpy(line_arr[line_index + 1].called_label, R_TYPE_NO_LABEL, MAX_LABEL_NAME_LENGTH);
				line_index += 2; // Increment line index for the opcode line (two lines are used for Label_call instruction)
			}
			else {
				line_index++; // Increment line index for the next line
			}
		}
		// if Non of the above - wrong line classification
		else {
			printf("Error (assembler): Invalid instruction format: %s\n", trimmed_asm_str);
			return -1; // Error: // Error: If there is no comma before the imm string the format is invalid (and we were told to assume correctness)
		}
		
	}
	*asm_line_count = line_index;
	*label_count = label_dict_index;
	return 0; // or -1 on error
}

int second_pass(Line* line_arr, int line_count, Label* label_dict, int label_count, uint32_t* memin, unsigned int* word_is_zero_location) {
	/*
	 Perform the second pass of the assembler to fill the imm32 field in Imm32 lines and convert lines to hexadecimal representation.
	 @param line_arr: Pointer to the array of Line structs containing parsed assembly lines.
	 @param line_count: Number of lines in the line array.
	 @param label_dict: Pointer to the array of Label structs containing label addresses.
	 @param label_count: Number of labels in the label dictionary.
	 @param memin: Pointer to the memory input array to fill with machine code.
	 @param word_is_zero_location: Pointer to an array to track if a word line data is zero.
	 @return: int - Returns 0 on success, -1 on error.
	*/
	if (line_arr == NULL || label_dict == NULL || line_count <= 0 || label_count <= 0 || word_is_zero_location == NULL) {
		return -1; // Error: Invalid input
	}
	for (int i = 0; i < line_count; i++) {
		// Check if the line is a valid type and insert it to memin array
		if (line_arr[i].type == R) {
			// If the line is an R-type instruction, check if it has a bigimm set to 1
			if (line_arr[i].bigimm == 1) {
				// If it does, the next line should be an Imm32 line
				if (i + 1 >= line_count || line_arr[i + 1].type != Imm32) {
					printf("Error (assembler): Expected Imm32 line after R-type instruction with bigimm set to 1 at line %d\n", i);
					return -1; // Error: Expected Imm32 line after R-type instruction with bigimm set to 1
				}
			}
			memin[i] = R_line_to_hex(&(line_arr[i])); // Convert the R line to hex and store it in the memin array
		}

		else if (line_arr[i].type == Imm32) {
			// If the line is an Imm32 line, check if it has a label
			if (strncmp(line_arr[i].called_label, R_TYPE_NO_LABEL, MAX_LABEL_NAME_LENGTH) != 0) {
				// Find the corresponding label in the label dictionary
				bool label_found = false;
				for (int j = 0; j < label_count; j++) {
					if (strcmp(line_arr[i].called_label, label_dict[j].label_name) == 0) {
						// If found, set the imm32 field to the label address
						line_arr[i].imm32 = label_dict[j].label_address;
						label_found = true;
						break;
					}
				}
				if (!label_found) {
					printf("Error (assembler): Label '%s' not found in label dictionary at line %d\n", line_arr[i].called_label, i);
					return -1; // Error: Label not found in label dictionary
				}
			}
			// Convert it to hex and store it in the memin array
			memin[i] = Imm32_line_to_hex(&(line_arr[i])); // Convert the Imm32 line to hex and store it in the memin array
		}

		else if (line_arr[i].type == WORD) {
			// Store the data in the memin array at the address specified in the .word instruction
			memin[line_arr[i].word.address] = (line_arr[i].word.data & 0xFFFFFFFF);
			if (line_arr[i].word.data == 0) {
				word_is_zero_location[line_arr[i].word.address] = 1; // Mark the location that the data is zero
			}
			// Set the current line in the memin array to 0, as .word lines do not contribute to the instruction memory
			memin[i] = 0;
		}

		else if (line_arr[i].type == Label_call) {
			// If the line is an Label_call instruction, check if it has a bigimm set to 1
			if (line_arr[i].bigimm != 1 || strncmp(line_arr[i].called_label, R_TYPE_NO_LABEL, MAX_LABEL_NAME_LENGTH) == 0 || i + 1 >= line_count || line_arr[i + 1].type != Imm32) {
				printf("Error (assembler): Line instruction at line %d doesn't meet crateria for Label_call instruction at line\n", i);
				return -1; // Error: Line instruction doesn't meet criteria for Label_call instruction
			}
			memin[i] = R_line_to_hex(&(line_arr[i])); // Convert the R line to hex and store it in the memin array
		}

		else {
			printf("Error (assembler): Invalid line type at line %d\n", i);
			return -1; // Error: Invalid line type
		}
	}
	return 0; // Success
}