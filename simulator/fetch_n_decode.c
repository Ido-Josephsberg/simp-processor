#include <stdio.h>
#include <stdbool.h>
#include "isa_func.h"
#include "hwregtrace_handler.h"
#include "leds_handler.h"
#include "seg7display_handler.h"
#include "trace_handler.h"
#include "io_register.h"
#include "fetch_n_decode.h"


static void fetch_instruction(Simulator* sim) {
	/*
	Fetch the next instruction from memory. This function updates the instruction field in the simulator object based on the program counter.
	sim: The simulator object.
	*/

	// Check if the program counter is within bounds
	if (sim->pc < MEMORY_SIZE)
		// Fetch the next instruction from memory
		sim->inst = sim->memory[sim->pc];
	// If the program counter is out of bounds, print an error message and exit the program
	else {
		printf("Error: Program counter out of bounds.\n");
		exit(STACK_OVERFLOW_ERROR);
	}
}


static void decode_instruction(Simulator* sim) {
	/*
	Decode the fetched instruction and extract the opcode, rd, rs, rt, and immediate value.
	*/

	// Mask for the first byte (opcode)
	uint32_t byte_mask = 0xFF000000;
	// Decode the instruction to extract the opcode
	sim->opcode = (sim->inst & byte_mask) >> 24;

	// Mask for the register number (rd)
	uint32_t reg_num_mask = 0x00F00000;
	// Decode the instruction to extract the rd field.
	sim->rd = (sim->inst & reg_num_mask) >> 20;

	// Mask for the second register number (rs)
	reg_num_mask >>= 4;
	// Decode the instruction to extract the rs field.
	sim->rs = (sim->inst & reg_num_mask) >> 16;

	// Mask for the third register number (rt)
	reg_num_mask >>= 4;
	// Decode the instruction to extract the rt field.
	sim->rt = (sim->inst & reg_num_mask) >> 12;

	// Fetch the immediate value from the instruction
	uint8_t imm = sim->inst & 0xFF;
	// Shift the immediate value to the most significant byte position
	int32_t ext_imm = imm << (32 - IMM_NUM_OF_BYTES);
	// Arithmetic right shift to sign-extend the immediate value
	ext_imm >>= (32 - IMM_NUM_OF_BYTES);
	// Write the immediate value after sign extention to the register file
	write_register_imm(sim, ext_imm);

	// Mask for the bigimm field (bit index 8)
	uint32_t mask = 0x100;
	// Decode the bigimm value (zero or one).
	sim->bigimm = (sim->inst & mask) >> 8;
}

static void update_all_traces_n_execute(Simulator* sim) {
	/*
	Update relevant string for the output files and execute the instruction.
	sim: The simulator object.
	*/

	// Array of function pointers for the ISA functions
	void (*isa_funcs[])(Simulator*, reg_name, reg_name, reg_name) = { isa_add, isa_sub, isa_mul, isa_and,
		isa_or, isa_xor, isa_sll, isa_sra, isa_srl, isa_beq, isa_bne, isa_blt, isa_bgt, isa_ble, isa_bge,
		isa_jal, isa_lw, isa_sw, isa_reti, isa_in, isa_out, isa_halt };
	// Update the string of the trace file (before executing the instruction)
	update_trace(sim);
	// Execute the instruction using the appropriate ISA function based on the opcode
	isa_funcs[sim->opcode](sim, sim->rd, sim->rs, sim->rt);
	// If the instruction is IN or OUT (read or write to I/O registers), Update the string of the hardware register trace file
	if (sim->opcode == IN || sim->opcode == OUT) {
		update_hwregtrace(sim);
		// If the instruction is OUT, check if it is related to the 7-segment display or LEDs and update them accordingly
		if (sim->opcode == OUT) {
			int reg_index = read_register(sim, sim->rs) + read_register(sim, sim->rt);
			if (reg_index == DISPLAY7SEG)
				update_7seg(sim);
			if (reg_index == LEDS)
				update_leds(sim);
		}
	}
}


static void check_for_irq2(Simulator* sim, int* irq2_index_ptr) {
	/*
	Check if the current cycle matches the IRQ2 cycle
	*/

	// Check if the current cycle matches the IRQ2 cycle
	if (sim->cycles == ((int32_t*)sim->irq2_cycles->data)[*irq2_index_ptr]) {
		// If it does, set the IRQ2 status to 1
		write_io_reg(sim, IRQ2STATUS, 1);
		// If the IRQ2 index is less than the number of IRQ2 cycles, increment the index of the IRQ2 cycles.
		if (*irq2_index_ptr < sim->num_irq2_cycles - 1) {
			// Increment the index to the next IRQ2 cycle
			(*irq2_index_ptr)++;
		}
	}
}

static void update_timer(Simulator* sim) {
	/*
	Increment the timer value if the timer is enabled, and check if it has reached its maximum value.
	If it has, set the IRQ0 status to 1 and reset the timer value to 0.
	sim: The simulator object.
	*/

	// Check if the timer is enabled
	if (read_io_reg(sim, TIMERENABLE)) {
		// Get the current timer value and check if it is less than the maximum timer value.
		int32_t timer_value = read_io_reg(sim, TIMERCURRENT);
		if (timer_value < read_io_reg(sim, TIMERMAX)) {
			// If it is, increment the timer value and write it back to the timer register
			timer_value++;
			write_io_reg(sim, TIMERCURRENT, timer_value);
		}
		// If the timer value has reached the maximum value, set the IRQ0 status to 1 and reset the timer value to 0
		else {
			write_io_reg(sim, IRQ0STATUS, 1);
			write_io_reg(sim, TIMERCURRENT, 0);
		}
	}
}

static void check_disk_avilability(Simulator* sim) {
	/*
	Check if the disk is available for reading or writing. The disk is available every 1024 cycles.
	If the disk is available, set the IRQ1 status to 1.
	sim: The simulator object.
	*/
	if (sim->cycles % 1024 == 0)
		write_io_reg(sim, IRQ1STATUS, 1);
}

void fetch_n_decode_loop(Simulator* sim) {
	/*
	Fetch and decode loop for the simulator. This function runs until the simulator is not running.
	This function fetches the next instruction, decodes it, and executes it. It also takes care of interrupts and updates the strings for the output files.
	sim: The simulator object.
	*/

	// Initialize the IRQ flags
	bool irq0, irq1, irq2, irq;
	// Initialize the IRQ2 cycle index.
	int irq2_index = 0;
	
	// Keep running the loop until the simulator is not running
	while (sim->is_running) {
		printf("Cycle %d, PC: %d\n", sim->cycles, sim->pc);
		//if (sim->cycles == sim->)
		// TODO: REMOVE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!sim->bigimm) {
			// If the instruction is not a big immediate instruction, check for interrupts
			irq0 = read_io_reg(sim, IRQ0ENABLE) & read_io_reg(sim, IRQ0STATUS);
			irq1 = read_io_reg(sim, IRQ1ENABLE) & read_io_reg(sim, IRQ1STATUS);
			irq2 = read_io_reg(sim, IRQ2ENABLE) & read_io_reg(sim, IRQ2STATUS);
			irq = irq0 || irq1 || irq2;
			// check if there was an interruptand if the simulator is already in an IRQ handler.
			if (irq && (!sim->is_in_irq)) {
				// If there is an interrupt and the simulator is not already in an IRQ handler, indicate that the simulator is in an IRQ handler.
				sim->is_in_irq = true;
				// Update the irqreturn register with the current program counter value
				write_io_reg(sim, IRQRETURN, sim->pc);
				// Set the program counter to the IRQ handler address
				sim->pc = read_io_reg(sim, IRQHANDLER);
			}

			// Fetch the instruction
			fetch_instruction(sim);
			// Decode the instruction
			decode_instruction(sim);
			// Check if the instruction is a big immediate instruction
			if (!sim->bigimm) {
				// If it is not a big immediate instruction, update all traces and execute the instruction
				update_all_traces_n_execute(sim);
			}
		}
		// If the last instruction was a big immediate instruction, get the immediate value from the memory in current program counter, 
		// update the string of the output files, and execute the instruction.
		else {
			write_register_imm(sim, sim->memory[sim->pc]);
			update_all_traces_n_execute(sim);
			// Reset bigimm for the next instruction
			sim->bigimm = 0;
		}
		// Check if the pc has changed during the instruction execution (e.g., due to a jump or branch instruction).
		if (sim->is_pc_changed)
			// If the pc has changed, reset the is_pc_changed flag to false
			sim->is_pc_changed = false;
		else
			// If the pc has not changed, increment the program counter to point to the next instruction
			sim->pc++;
		// Update and check IRQs.
		check_for_irq2(sim, &irq2_index);
		update_timer(sim);
		check_disk_avilability(sim);

		// Increment the cycle count.
		sim->cycles++;
	}
}