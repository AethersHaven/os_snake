#include "cpu/isr.h"
#include "mem.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "drivers/timer.h"
#include "util/rand.h"
#include "util/console.h"

void core_init()
{
    isr_install();
    asm volatile("sti");
    init_heap();
    init_screen();
    init_keyboard();
    init_timer();
    srand();
    init_console();
}