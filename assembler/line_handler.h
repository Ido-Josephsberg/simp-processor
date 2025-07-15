#ifndef LINE_H
#define LINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_ASM_LINE_LENGTH 500				// max length of a single assembly line as specified in the project
#define MAX_MEM_LINES 4069					// memory depth is max 4096 lines.
#define MAX_LABEL_NAME_LENGTH 50			// max length of label name
#define R_TYPE_NO_LABEL "R-type no label"	// label name for R-type instruction
#define MIN_8BIT_VALUE -128					// minimum value for 8-bit signed integer
#define MAX_8BIT_VALUE 127					// maximum value for 8-bit signed integer

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
	R,				// R-type instruction - no label, instruction that contain a number in the imm field
	Label_call,		// label line - a line imm == label (if condition hold it jump to label address)
	Imm32,			// 32-bit immediate value (extra line if prev line has big-imm == 1)
	WORD,
} LineType;

typedef struct {
	char label_name[MAX_LABEL_NAME_LENGTH];	 // Label name, max length is MAX_LABEL_NAME_LENGTH
	uint32_t label_address;					 // 32 bits for label address
}Label;

typedef struct {
	uint32_t address;
	int32_t data;
} Word;

typedef struct Line {
	// updade on line read
	LineType type;
	// 1st byte:
	uint8_t opcode;								// 2 hex
	// 2nd byte:
	uint8_t rd : 4;								// 1 hex
	uint8_t rs : 4;								// 1 hex
	// 3rd byte:
	uint8_t rt : 4;								// 1 hex
	uint8_t reserved : 3;						// 3 bits reserved for future use
	uint8_t bigimm : 1;							// 1 bit flag for big immediate
	// 4th byte:
	int8_t imm8;								// 1 hex, 8 bits for small immediate, used if bigimm == 0	
	// elif Imm32
	int32_t imm32;								// 4 hex, 32 bits for large immediate, used if bigimm == 1
	// else Label
	char called_label[MAX_LABEL_NAME_LENGTH];	// Label name, that this line calls to.
	// if .word instruction
	Word word;									// .word instruction struct
} Line;

// Function declerations:
int parse_number(const char* str, int32_t* out_num);
int parse_instruction_to_line(char* asm_str, Line* line, LineType type);

#endif // !LINE_H