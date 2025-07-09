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
	// Fetch the next instruction from memory
	if (sim->pc < MEMORY_SIZE)
		sim->inst = sim->memory[sim->pc];

	else {
		printf("Error: Program counter out of bounds.\n");
		sim->is_running = false;
	}
}


static void decode_instruction(Simulator* sim) {
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
	void (*isa_funcs[])(Simulator*, reg_name, reg_name, reg_name) = { isa_add, isa_sub, isa_mul, isa_and,
		isa_or, isa_xor, isa_sll, isa_sra, isa_srl, isa_beq, isa_bne, isa_blt, isa_bgt, isa_ble, isa_bge,
		isa_jal, isa_lw, isa_sw, isa_reti, isa_in, isa_out, isa_halt };

	update_trace(sim);
	isa_funcs[sim->opcode](sim, sim->rd, sim->rs, sim->rt);
	if (sim->opcode == IN || sim->opcode == OUT) {
		update_hwregtrace(sim);
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
	if (sim->cycles == ((int32_t*)sim->irq2_cycles->data)[*irq2_index_ptr]) {
		write_io_reg(sim, IRQ2STATUS, 1);
		if (*irq2_index_ptr < sim->num_irq2_cycles - 1) {
			// Increment the index to the next IRQ2 cycle
			(*irq2_index_ptr)++;
		}
	}
}

static void update_timer(Simulator* sim) {
	// Check if the timer is enabled
	if (read_io_reg(sim, TIMERENABLE)) {
		// Decrement the timer value
		int32_t timer_value = read_io_reg(sim, TIMERCURRENT);
		if (timer_value < read_io_reg(sim, TIMERMAX)) {
			timer_value++;
			write_io_reg(sim, TIMERCURRENT, timer_value);
		}
		else {
			write_io_reg(sim, IRQ1STATUS, 1);
			write_io_reg(sim, TIMERCURRENT, 0);
		}
	}
}

static void check_disk_avilability(Simulator* sim) {
	if (sim->cycles % 1024 == 0)
		write_io_reg(sim, IRQ1STATUS, 1);
}

void fetch_n_decode_loop(Simulator* sim) {
	bool irq0, irq1, irq2, irq;
	int irq2_index = 0;
	void (*isa_funcs[])(Simulator*, reg_name, reg_name, reg_name) = { isa_add, isa_sub, isa_mul, isa_and,
		isa_or, isa_xor, isa_sll, isa_sra, isa_srl, isa_beq, isa_bne, isa_blt, isa_bgt, isa_ble, isa_bge,
		isa_jal, isa_lw, isa_sw, isa_reti, isa_in, isa_out, isa_halt };
	
	while (sim->is_running) {
		printf("cycle: %08X pc: %03X instruction: %08X  reg_14: %08X\n", sim->cycles, sim->pc, sim->inst, sim->reg_array[14]);
		//if (sim->cycles == sim->)
		// TODO: REMOVE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!sim->bigimm) {
			// Check for interrupts
			irq0 = read_io_reg(sim, IRQ0ENABLE) & read_io_reg(sim, IRQ0STATUS);
			irq1 = read_io_reg(sim, IRQ1ENABLE) & read_io_reg(sim, IRQ1STATUS);
			irq2 = read_io_reg(sim, IRQ2ENABLE) & read_io_reg(sim, IRQ2STATUS);
			irq = irq0 || irq1 || irq2;
			if (irq && (!sim->is_in_irq)) {
				sim->is_in_irq = true;
				write_io_reg(sim, IRQRETURN, sim->pc);
				sim->pc = read_io_reg(sim, IRQHANDLER);
			}

			// Fetch the instruction
			fetch_instruction(sim);
			if (sim->cycles == 117) {
				printf("Debug\n");
			}
			decode_instruction(sim);
			if (!sim->bigimm) {
				// TODO: remove this debug print statement in production code!!!!!!!!!!!!!!!!
				update_all_traces_n_execute(sim);
			}
		}
		else {
			// TODO: remove this debug print statement in production code!!!!!!!!!!!!!!!!
			write_register_imm(sim, sim->memory[sim->pc]);
			update_all_traces_n_execute(sim);
			// Reset bigimm for the next instruction
			sim->bigimm = 0;
		}
		if (sim->is_pc_changed)
			sim->is_pc_changed = false;
		else
			sim->pc++;
		// Update and check IRQs.
		check_for_irq2(sim, &irq2_index);
		update_timer(sim);
		check_disk_avilability(sim);

		sim->cycles++;
	}
}