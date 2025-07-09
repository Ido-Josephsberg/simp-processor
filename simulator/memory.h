#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>


// forward declaration of Simulator structure
struct Simulator;
typedef struct Simulator Simulator;

// Memory Related Functions
void init_memory(Simulator* sim);
int32_t read_memory(Simulator* sim, int32_t address);
int write_memory(Simulator* sim, int32_t address, int32_t value);
int write_memout_file_wrapper(Simulator* sim, output_paths* paths);

#endif // MEMORY_H
