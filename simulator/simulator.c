#include "simulator.h"
#include "register.h"
#include "io_register.h"
#include "monitor.h"
#define True 1
#define False 0

void simulator_init(Simulator* sim, input_paths in_paths, output_paths out_paths) {
	sim->is_running = True;
	sim->is_pc_changed = False;
	sim->is_in_irq = False;
	sim->pc = 0;
	sim->bigimm = 0;
	init_reg_array(sim);
	init_io_reg_array(sim);
	sim->max_memory_address = 0;
	init_memory(sim);
	read_memin_file(&in_paths, sim->memory, &(sim->max_memory_address));
	sim->cycles = 0;
	read_diskin_file(&in_paths, sim->disk);
    sim->max_monitor_pixel[0] = 0;  
    sim->max_monitor_pixel[1] = 0;
	init_monitor(sim);
	sim->trace_str = create_dynamic_mem();
	sim->hwregtrace_str = create_dynamic_mem();
	sim->leds_str = create_dynamic_mem();
	sim->seg7display_str = create_dynamic_mem();
	sim->num_irq2_cycles = 0;
	sim->irq2_cycles = create_dynamic_mem();
	read_irq2in_file(&in_paths, sim->irq2_cycles, &(sim->num_irq2_cycles));
}