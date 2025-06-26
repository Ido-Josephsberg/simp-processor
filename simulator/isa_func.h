#pragma once


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
