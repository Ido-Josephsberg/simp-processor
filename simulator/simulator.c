#include "simulator.h"
#include "register.h"
#include "io_register.h"
#include "monitor.h"
#define True 1
#define False 0

static void free_after_hw(Simulator* sim) {
	/*
	Free the memory allocated for the simulator after hwregtrace memory allocation was failed.
	sim: Pointer to the Simulator structure to be freed.
	*/
	if (sim != NULL) {
		free_dynamic_mem(sim->trace_str);
		free(sim);
	}
}
static void free_after_leds(Simulator* sim) {
	/*
	Free the memory allocated for the simulator after leds memory allocation was failed.
	sim: Pointer to the Simulator structure to be freed.
	*/
	if (sim != NULL) {
		free_dynamic_mem(sim->hwregtrace_str);
		free_after_hw(sim);
	}
}
static void free_after_seg7display(Simulator* sim) {
	/*
	Free the memory allocated for the simulator after seg7display memory allocation was failed.
	sim: Pointer to the Simulator structure to be freed.
	*/
	if (sim != NULL) {
		free_dynamic_mem(sim->leds_str);
		free_after_leds(sim);
	}
}

static void free_after_irq2_cycles(Simulator* sim) {
	/*
	Free the memory allocated for the simulator after irq2_cycles memory allocation was failed.
	sim: Pointer to the Simulator structure to be freed.
	*/
	if (sim != NULL) {
		free_dynamic_mem(sim->seg7display_str);
		free_after_seg7display(sim);
	}
}

void free_simulator(Simulator* sim) {
	/*
	Free the memory allocated for the simulator and its components.
	sim: Pointer to the Simulator structure to be freed.
	*/

	if (sim != NULL) {
		free_dynamic_mem(sim->trace_str);
		free_dynamic_mem(sim->hwregtrace_str);
		free_dynamic_mem(sim->leds_str);
		free_dynamic_mem(sim->seg7display_str);
		free_dynamic_mem(sim->irq2_cycles);
		free(sim);
	}
}

Simulator* simulator_init(input_paths in_paths) {
	/*
	Initialize the simulator object and allocate memory for the simulator and its components.
	in_paths: Input paths for reading memory and disk files.
	*/

	// Allocate memory for the Simulator structure and initialize its fields to zero.
	Simulator* sim = calloc(1, sizeof(Simulator));
	// Check if memory allocation was successful
	if (sim == NULL) {
		printf("Error allocating memory for Simulator\n");
		return NULL;
	}

	// Initialize the simulator's fields. For dynamic fields, check if memory allocation was successful. If not, free the simulator and return NULL.
	sim->is_running = True;
	sim->is_pc_changed = False;
	sim->is_in_irq = False;
	if (read_memin_file(&in_paths, sim->memory, &(sim->max_memory_address)) == -1) {
		printf("Error reading memory input file\n");
		free(sim);
		return NULL;
	}
	if (read_diskin_file(&in_paths, sim->disk) == -1) {
		printf("Error reading disk input file\n");
		free(sim);
		return NULL;
	}
	sim->trace_str = create_dynamic_mem();
	if (sim->trace_str == NULL) {
		printf("Error allocating memory for trace_str\n");
		free(sim);
		return NULL;
	}
	sim->hwregtrace_str = create_dynamic_mem();
	if (sim->hwregtrace_str == NULL) {
		printf("Error allocating memory for hwregtrace_str\n");
		free_after_hw(sim);
		return NULL;
	}
	sim->leds_str = create_dynamic_mem();
	if (sim->leds_str == NULL) {
		printf("Error allocating memory for leds_str\n");
		free_after_leds(sim);
		return NULL;
	}
	sim->seg7display_str = create_dynamic_mem();
	if (sim->seg7display_str == NULL) {
		printf("Error allocating memory for seg7display_str\n");
		free_after_seg7display(sim);
		return NULL;
	}
	sim->num_irq2_cycles = 0;
	sim->irq2_cycles = create_dynamic_mem();
	if (sim->irq2_cycles == NULL) {
		printf("Error allocating memory for irq2_cycles\n");
		free_after_irq2_cycles(sim);
		return NULL;
	}

	if(read_irq2in_file(&in_paths, sim->irq2_cycles, &(sim->num_irq2_cycles)) == -1) {
		free_simulator(sim);
		return NULL;
	}
	// Return pointer to the initialized simulator object
	return sim;
}

