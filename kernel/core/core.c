#include "cpu/isr.h"
#include "mem.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "util/rand.h"
#include "util/console.h"

void core_init()
{
    isr_install();
    asm volatile("sti");
    init_heap();
    srand();
    init_screen();
    init_console();
    init_keyboard();
}