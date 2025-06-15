#include "register.h"

// 0. Implementation of the "ADD" ISA instruction
static void isa_add(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(rf, rs) + read_register(rf, rt);
	write_register(rf, rd, result);
}

// 1. Implementation of the "SUB" ISA instruction
static void isa_sub(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(rf, rs) - read_register(rf, rt);
	write_register(rf, rd, result);
}

// 2. Implementation of the "MUL" ISA instruction
static void isa_mul(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(rf, rs) * read_register(rf, rt);
	write_register(rf, rd, result);
}

// 3. Implementation of the "and" ISA instruction
static void isa_and(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(rf, rs) & read_register(rf, rt);
	write_register(rf, rd, result);
}

// 4. Implementation of the "or" ISA instruction
static void isa_or(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(rf, rs) | read_register(rf, rt);
	write_register(rf, rd, result);
}

// 5. Implementation of the "xor" ISA instruction
static void isa_xor(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(rf, rs) ^ read_register(rf, rt);
	write_register(rf, rd, result);
}

// 6. Implementation of the "sll" ISA instruction
static void isa_sll(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(rf, rs) << read_register(rf, rt);
	write_register(rf, rd, result);
}

// 7. Implementation of the "sra" ISA instruction
static void isa_sra(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(rf, rs) >> read_register(rf, rt);
	write_register(rf, rd, result);
}

// 8. Implementation of the "srl" ISA instruction
static void isa_srl(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	// logical right shift: cast to uint_32 to make sure shifting pads with zeros
	int32_t result = (uint32_t)read_register(rf, rs) >> read_register(rf, rt);
	write_register(rf, rd, result);
}

// 9. Implementation of the "beq" ISA instruction
static void isa_beq(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(rf, rs) == read_register(rf, rt)) {
		// Branch taken, adjust the program counter (PC)
		pc = read_register(rf, rd);														// Assuming 'pc' is a global variable representing the program counter
	}
}

// 10. Implementation of the "bne" ISA instruction
static void isa_bne(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(rf, rs) != read_register(rf, rt)) {
		// Branch taken, adjust the program counter (PC)
		pc = read_register(rf, rd); 													// Assuming 'pc' is a global variable representing the program counter
	}
}

// 11. Implementation of the "blt" ISA instruction
static void isa_blt(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(rf, rs) < read_register(rf, rt)) {
		// Branch taken, adjust the program counter (PC)
		pc = read_register(rf, rd); 													// Assuming 'pc' is a global variable representing the program counter
	}
}

// 12. Implementation of the "bgt" ISA instruction
static void isa_bgt(reg_file* rf, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(rf, rs) > read_register(rf, rt)) {
		// Branch taken, adjust the program counter (PC)
		pc = read_register(rf, rd); 													// Assuming 'pc' is a global variable representing the program counter
	}
}