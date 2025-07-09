#ifndef LINE_H
#define LINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LABEL_NAME_LENGTH 50 // max length of label name
#define R_TYPE_NO_LABEL "R-type no label" // label name for R-type instruction

///////////////////////////////////// TODO /////////////////////////////////////
//  # If there is ----------------------------> please take a look	
//  # questions:
// 		1. word.data - can be negative? -- FOR NOW I ASSUME IT CAN BE NEGATIVE
////////////////////////////////////////////////////////////////////////////////

typedef enum { // 4 bits for register code
	zero,				//0
	imm,				//1
	v0,					//2
	a0, a1, a2, a3,     //3, 4, 5, 6
	t0, t1, t2,			//7, 8, 9
	s0, s1, s2,			//10, 11, 12
	gp, sp, ra			//13, 14, 15		
} reg_code;

typedef enum {
	add,	//0
	sub,	//1
	mul,	//2
	and,	//3
	or,		//4
	xor,	//5
	sll,	//6
	sra,	//7
	srl,	//8
	beq,	//9
	bne,	//10
	blt,	//11
	bgt,	//12
	ble,	//13
	bge,	//14
	jal,	//15
	lw,		//16
	sw,		//17
	reti,	//18
	in,		//19
	out,	//20
	halt,	//21
} opcode;

typedef enum {
	R,
	Label_call,		// label line - a line imm == label (if condition hold it jump to label address)
	Imm32,			// 32-bit immediate value (extra line if prev line has big-imm == 1)
	WORD,
} LineType;

typedef struct {
	char label_name[MAX_LABEL_NAME_LENGTH];	// 32 chars for label name - is it enough?
	uint32_t label_address;	// 4 hex, 32 bits for label address
}Label;

typedef struct {
	uint32_t address;
	int32_t data;	//-------------------------------------------------------------> Q: the number of data can be negative?
} Word;

typedef struct Line {
	// updade on line read
	LineType type;
	// 1st byte:
	uint8_t opcode;					// 2 hex
	// 2nd byte:
	uint8_t rd : 4;					// 1 hex
	uint8_t rs : 4;					// 1 hex
	// 3rd byte:
	uint8_t rt : 4;					// 1 hex
	uint8_t reserved : 3;			// 3 bits reserved for future use
	uint8_t bigimm : 1;				// 1 bit flag for big immediate
	// 4th byte:
	uint8_t imm8;					// 1 hex, 8 bits for small immediate	
	// elif Imm32
	uint32_t imm32;					// 4 hex, 32 bits for large immediate
	// else Label
	char called_label[MAX_LABEL_NAME_LENGTH];				// label struct for label lines
	// if .word instruction
	Word word;					    // .word instruction struct
} Line;

// Function declerations:

int parse_number(const char* str);
static int init_word_struct(Word* word, char* addr_data_str);
static int get_register_code(const char* reg_str);
static int get_opcode_code(const char* opcode_str);
static int init_opcode_and_registers(char* asm_str, Line* line);
int parse_instruction_to_line(char* asm_str, Line* line, LineType type);

#endif // !LINE_H