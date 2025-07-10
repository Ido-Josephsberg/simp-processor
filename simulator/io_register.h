#ifndef IO_REGISTER_H
#define IO_REGISTER_H

#include <stdint.h>
#include <stdlib.h>



// forward declaration of Simulator structure
struct Simulator;
typedef struct Simulator Simulator;

typedef enum {
    IRQ0ENABLE,     // 0
    IRQ1ENABLE,	    // 1
    IRQ2ENABLE,     // 2
    IRQ0STATUS,     // 3
    IRQ1STATUS,     // 4
    IRQ2STATUS,     // 5
    IRQHANDLER,     // 6
    IRQRETURN,      // 7
    CLKS,           // 8
    LEDS,           // 9
    DISPLAY7SEG,    // 10
    TIMERENABLE,    // 11
    TIMERCURRENT,   // 12
    TIMERMAX,       // 13
    DISKCMD,        // 14
    DISKSECTOR,     // 15
    DISKBUFFER,     // 16
    DISKSTATUS,     // 17
    RESERVED18,     // 18
    RESERVED19,     // 19
    MONITORADDR,    // 20
    MONITORDATA,    // 21
    MONITORCMD      // 22
} io_reg_name;


int get_io_reg_name(char* dst, io_reg_name reg_index);
int get_io_reg_length(io_reg_name reg_index);
int32_t read_io_reg(Simulator* sim, io_reg_name reg_index);
void write_io_reg(Simulator* sim, io_reg_name reg_index, int32_t value);

#endif // IO_REGISTER_H