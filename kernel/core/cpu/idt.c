#include "idt.h"

#include "../mem.h"

void set_idt_gate(int n, unsigned int handler)
{
    idt[n].low_offset = low_16(handler);
    idt[n].selector = KERNEL_CS;
    idt[n].zero = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void set_idt()
{
    idt_register.base = (unsigned int)&idt;
    idt_register.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    // Don't make the mistake of loading &idt -- always load &idt_register
    __asm__ __volatile__("lidtl (%0)" : : "r"(&idt_register));
}

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_register;