#include "register.h"
#include "io_register.h"
#include "simulator.h"
#include "isa_func.h"

// Update the monitor with the pixel value read from the MONITORDATA register
static void update_monitor(Simulator* sim, int32_t offset) {
	/*
	Update the monitor with the pixel value read from the MONITORDATA register at the specified offset.
	sim: The pointer to the simulator struct.
	offset: The offset to determine the row and column in the monitor.
	*/
	int32_t row = offset / PIXEL_PER_ROW_COL;
	int32_t col = offset % PIXEL_PER_ROW_COL;
	int32_t pixel_value = read_io_reg(sim, MONITORDATA);
	sim->monitor[row][col] = pixel_value;
	if (row >= sim->max_monitor_pixel[0] && col > sim->max_monitor_pixel[1] && pixel_value){
		// Update the maximum pixel position used in the monitor
		sim->max_monitor_pixel[0] = row;
		sim->max_monitor_pixel[1] = col;
	}
}

// 0. Implementation of the "ADD" ISA instruction
void isa_add(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) + read_register(sim, rt);
	write_register(sim, rd, result);
}

// 1. Implementation of the "SUB" ISA instruction
void isa_sub(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) - read_register(sim, rt);
	write_register(sim, rd, result);
}

// 2. Implementation of the "MUL" ISA instruction
void isa_mul(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) * read_register(sim, rt);
	write_register(sim, rd, result);
}

// 3. Implementation of the "and" ISA instruction
void isa_and(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) & read_register(sim, rt);
	write_register(sim, rd, result);
}

// 4. Implementation of the "or" ISA instruction
void isa_or(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) | read_register(sim, rt);
	write_register(sim, rd, result);
}

// 5. Implementation of the "xor" ISA instruction
void isa_xor(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) ^ read_register(sim, rt);
	write_register(sim, rd, result);
}

// 6. Implementation of the "sll" ISA instruction
void isa_sll(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) << read_register(sim, rt);
	write_register(sim, rd, result);
}

// 7. Implementation of the "sra" ISA instruction
void isa_sra(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t result = read_register(sim, rs) >> read_register(sim, rt);
	write_register(sim, rd, result);
}

// 8. Implementation of the "srl" ISA instruction
void isa_srl(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	// logical right shift: cast to uint_32 to make sure shifting pads with zeros
	int32_t result = (uint32_t)read_register(sim, rs) >> read_register(sim, rt);
	write_register(sim, rd, result);
}

// 9. Implementation of the "beq" ISA instruction
void isa_beq(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) == read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
		// Set the flag to indicate that the program counter has changed
		sim->is_pc_changed = 1;
	}
}

// 10. Implementation of the "bne" ISA instruction
void isa_bne(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) != read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
		// Set the flag to indicate that the program counter has changed
		sim->is_pc_changed = 1;
	}
}

// 11. Implementation of the "blt" ISA instruction
void isa_blt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) < read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
		// Set the flag to indicate that the program counter has changed
		sim->is_pc_changed = 1;
	}
}

// 12. Implementation of the "bgt" ISA instruction
void isa_bgt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) > read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
		// Set the flag to indicate that the program counter has changed
		sim->is_pc_changed = 1;
	}
}

// 13. Implementation of the "ble" ISA instruction
void isa_ble(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) <= read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
		// Set the flag to indicate that the program counter has changed
		sim->is_pc_changed = 1;
	}
}

// 14. Implementation of the "bge" ISA instruction
void isa_bge(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	if (read_register(sim, rs) >= read_register(sim, rt)) {
		// Branch taken, adjust the program counter (sim->pc)
		sim->pc = read_register(sim, rd) & 0xfff;	// 12 bit mask for sim->pc
		// Set the flag to indicate that the program counter has changed
		sim->is_pc_changed = 1;
	}
}

// 15. Implementation of the "jal" ISA instruction
void isa_jal(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	// Save the return address in the link register (rd should be REG_RA)
	write_register(sim, rd, sim->pc + 1); // Save the next instruction address
	// Jump to the target address
	sim->pc = read_register(sim, rs) & 0xfff; // 12 bit mask for sim->pc
	// Set the flag to indicate that the program counter has changed
	sim->is_pc_changed = 1;
}

// 16. Implementation of the "lw" ISA instruction
void isa_lw(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t address = read_register(sim, rs) + read_register(sim, rt);
	int32_t value = read_memory(sim, address);
	write_register(sim, rd, value);
}


// 17. Implementation of the "sw" ISA instruction
void isa_sw(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t address = read_register(sim, rs) + read_register(sim, rt);
	int32_t value = read_register(sim, rd);
	write_memory(sim, address, value);
	if (address > sim->max_memory_address && value != 0) {
		// Update the maximum memory address used
		sim->max_memory_address = address;
	}
}

// 18. Implementation of the "reti" ISA instruction	(interupt return address)
void isa_reti(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t return_address = read_io_reg(sim, IRQRETURN);
	sim->pc = return_address;
	// Set the flag to indicate that the program counter has changed
	sim->is_pc_changed = 1;
	// Clear the IRQ flag
	sim->is_in_irq = 0;
}

// 19. Implementation of the "in" ISA instruction
void isa_in(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t io_reg_index = read_register(sim, rs) + read_register(sim, rt);
	// If the io_reg_index is MONITORCMD, we always read 0.
	int32_t value = io_reg_index == MONITORCMD ? 0 : read_io_reg(sim, io_reg_index);
	write_register(sim, rd, value);
}

// 20. Implementation of the "out" ISA instruction
void isa_out(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	int32_t io_reg_index = read_register(sim, rs) + read_register(sim, rt);
	int32_t value = read_register(sim, rd);
	// If the io_reg_index is MONITORCMD, we update the monitor with the value from MONITORADDR register
	if (io_reg_index == MONITORCMD && value == 1)
		update_monitor(sim, read_io_reg(sim, MONITORADDR));
	write_io_reg(sim, io_reg_index, value);
}

// 21. Implementation of the "halt" ISA instruction
void isa_halt(Simulator* sim, reg_name rd, reg_name rs, reg_name rt) {
	// Set the simulator's running flag to false to stop execution
	sim->is_running = 0;
}