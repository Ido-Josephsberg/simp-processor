#pragma once
#include "simulator.h"

void fetch_instruction(Simulator* sim);
void decode_instruction(Simulator* sim);
void fetch_n_decode_loop(Simulator* sim);