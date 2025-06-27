#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "io_register.h"



void get_io_reg_name(char* dst, int index) {
	/*
	Write the name of the io register to dst based on the index.
	dst: Pointer to a string buffer where the name will be written. should be at least IO_REG_MAX_NAME_SIZE characters long.
	index: The index of the io register to copy his name.
	*/

	// Array of io register names ordered by their indices.
	const char* hw_reg_names[] = { "irq0enable", "irq1enable", "irq2enable", "irq0status",
		"irq1status", "irq2status", "irqhandler", "irqreturn", "clks", "leds", "display7seg",
		"timerenable", "timercurrent", "timermax", "diskcmd", "disksector" };
	// Copy the name of the io register to the destination string.
	strcpy(dst, hw_reg_names[index]);
}