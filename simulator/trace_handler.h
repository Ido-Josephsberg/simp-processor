#pragma once
#define TRACE_LINE_SIZE 167 // 165 + newline + null terminator


void update_trace(Simulator* sim);
int write_trace_file_wrapper(Simulator* sim, output_paths* paths);