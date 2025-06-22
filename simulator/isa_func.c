#include "register.h"
#include "simulator.h"

// 0. Implementation of the "ADD" ISA instruction
static void isa_add(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) + read_register(sim, rt);
	write_register(sim, rd, result);
}

// 1. Implementation of the "SUB" ISA instruction
static void isa_sub(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) - read_register(sim, rt);
	write_register(sim, rd, result);
}

// 2. Implementation of the "MUL" ISA instruction
static void isa_mul(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) * read_register(sim, rt);
	write_register(sim, rd, result);
}

// 3. Implementation of the "and" ISA instruction
static void isa_and(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) & read_register(sim, rt);
	write_register(sim, rd, result);
}

// 4. Implementation of the "or" ISA instruction
static void isa_or(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) | read_register(sim, rt);
	write_register(sim, rd, result);
}

// 5. Implementation of the "xor" ISA instruction
static void isa_xor(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) ^ read_register(sim, rt);
	write_register(sim, rd, result);
}

// 6. Implementation of the "sll" ISA instruction
static void isa_sll(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) << read_register(sim, rt);
	write_register(sim, rd, result);
}

// 7. Implementation of the "sra" ISA instruction
static void isa_sra(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) >> read_register(sim, rt);
	write_register(sim, rd, result);
}

// 8. Implementation of the "srl" ISA instruction
static void isa_srl(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	// logical right shift: cast to uint_32 to make sure shifting pads with zeros
	int32_t result = (uint32_t)read_register(sim, rs) >> read_register(sim, rt);
	write_register(sim, rd, result);
}

// 9. Implementation of the "beq" ISA instruction
static void isa_beq(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) == read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
	}
}

// 10. Implementation of the "bne" ISA instruction
static void isa_bne(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) != read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
	}
}

// 11. Implementation of the "blt" ISA instruction
static void isa_blt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) < read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
	}
}

// 12. Implementation of the "bgt" ISA instruction
static void isa_bgt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) > read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
	}
}

// 13. Implementation of the "ble" ISA instruction
static void isa_ble(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) <= read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
	}
}

// 14. Implementation of the "bge" ISA instruction
static void isa_bge(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) >= read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
	}
}

// 15. Implementation of the "jal" ISA instruction
static void isa_jal(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	// Save the return address in the link register (rd should be REG_RA)
	write_register(sim, rd, sim->pc + 1); // Save the next instruction address
	// Jump to the target address
	sim->pc = read_register(sim, rs) & 0xfff; // 12 bit mask for sim->pc
}

// 16. Implementation of the "lw" ISA instruction
static void isa_lw(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t address = read_register(sim, rs) + read_register(sim, rt);
	int32_t value = read_memory(sim, address);
	write_register(sim, rd, value);
}


// 17. Implementation of the "sw" ISA instruction
static void isa_sw(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t address = read_register(sim, rs) + read_register(sim, rt);
	int32_t value = read_register(sim, rd);
	write_memory(sim, address, value);
}

// 18. Implementation of the "reti" ISA instruction										# TODO # TODO # BOM
static void isa_reti(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {

}

// 19. Implementation of the "in" ISA instruction										# TODO # TODO # BOM
static void isa_in(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {

}

// 20. Implementation of the "out" ISA instruction										# TODO # TODO # BOM
static void isa_out(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	
}

// 21. Implementation of the "halt" ISA instruction										# Need to implement halt functionality on the simulator	file
static void isa_halt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	// Set the simulator's running flag to false to stop execution
	sim->is_running = 0;
}