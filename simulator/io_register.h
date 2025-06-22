#ifndef IO_REGISTER_H
#define IO_REGISTER_H

#include <stdint.h>
#include <stdlib.h>

#define IO_REG_NUM 23

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
    DISKSSECTOR,    // 15
    DISKBUFFER,     // 16
    DISKSTATUS,     // 17
    RESERVED18,     // 18
    RESERVED19,     // 19
    MONITORADDR,    // 20
    MONITORDATA,    // 21
    MONITORCMD      // 22
} io_reg_name;

// Register Related Functions
// TODO                                                                                 # TODO # TODO # BOM
//int32_t read_io_register(Simulator* sim, io_reg_name reg, ??????? );
//void write_io_register(Simulator* sim, io_reg_name reg, int32_t value, ???????);
// more functions as needed 


#endif // IO_REGISTER_H