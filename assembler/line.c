#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

include "line.h"

static uint32_t R_line_to_hex(const Line* line) {
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
	return (line->imm32 & 0xFFFFFFFF);
}

static uint32_t Label_line_to_hex(const Line* line) {
	return (line->label_address & 0xFFFFFFFF);
}

