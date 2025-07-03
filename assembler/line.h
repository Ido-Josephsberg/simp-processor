#ifndef LINE_H
# #define LINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// consider location for reg related part 

typedef enum { // 4 bits for register code
	zero,
	imm,
	v0, 
	a0, a1, a2, a3,
	t0, t1, t2,
	s0, s1, s2,
	gp, sp, ra
} reg_code;

//// --------------------------------------------------------------------> check if this is needed
//// convert reg_code to hex value
//static inline uint8_t reg_code_to_hex(reg_code reg) {
//	switch (reg) {
//		case zero: return 0x0;
//		case imm: return 0x1;
//		case v0: return 0x2;
//		case a0: return 0x3;
//		case a1: return 0x4;
//		case a2: return 0x5;
//		case a3: return 0x6;
//		case t0: return 0x7;
//		case t1: return 0x8;
//		case t2: return 0x9;
//		case s0: return 0xa;
//		case s1: return 0xb;
//		case s2: return 0xc;
//		case gp: return 0xd;
//		case sp: return 0xe;
//		case ra: return 0xf;
//		default: exit(EXIT_FAILURE); // invalid register code
//	}
//}

// ----

typedef enum {
	R,
	Imm32,		// 32-bit immediate value (extra line if prev line has big-imm == 1)
	Label,
} LineType;

typedef struct Line {
	// updade on line read
	line_type type;
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
	char label_name[32];			// 32 chars for label name	----> valide this assumption (32 chars is enough for label name) ----> can be set on first run
	uint32_t label_address;			// 4 hex, 32 bits for label address ----> to be set on second run
} Line;

// Simulator Related Functions

static inline uint32_t R_line_to_hex(const Line* line);
static inline uint32_t Imm32_line_to_hex(const Line* line);
static inline uint32_t Label_line_to_hex(const Line* line);



#endif // !LINE_H