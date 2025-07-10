#pragma once
#include "simulator.h"

typedef enum {
	ADD, // 0
	SUB, // 1
	MUL, // 2
	AND, // 3
	OR, // 4
	XOR, // 5
	SLL, // 6
	SRA, // 7
	SRL, // 8
	BEQ, // 9
	BNE, // 10
	BLT, // 11
	BGT, // 12
	BLE, // 13
	BGE, // 14
	JAL, // 15
	LW, // 16
	SW, // 17
	RETI, // 18
	IN, // 19
	OUT, // 20
	HALT // 21
}isa_opcode;

void isa_add(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_sub(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_mul(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_and(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_or(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_xor(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_sll(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_sra(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_srl(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_beq(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_bne(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_blt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_bgt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_ble(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_bge(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_jal(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_lw(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_sw(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_reti(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_in(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_out(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
void isa_halt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt);
