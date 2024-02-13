#include "../cpu/isr.h"
#include "ports.h"

unsigned int ticks = 0;

static void timer_callback(registers_t *regs)
{
    ticks++;
}

void init_timer()
{
    register_interrupt_handler(IRQ0, timer_callback);

    // Weird Qemu stuff, just run clock at ~1875 Hz
    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, 0x58);
    port_byte_out(0x40, 0x02);
}

float get_time()
{
    return ticks / 1875.0f;
}