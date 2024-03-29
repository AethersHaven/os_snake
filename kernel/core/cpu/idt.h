#ifndef IDT_H
#define IDT_H

#include "../mem.h"

#define KERNEL_CS 0x08

// How every interrupt gate (handler) is defined
typedef struct
{
    unsigned short low_offset; // Lower 16 bits of handler function address
    unsigned short selector;   // Kernel segment selector
    unsigned char zero;        // Always zero
    /*
    Bit 7: "Interrupt is present"
    Bits 6-5: Privilege level of caller (0=kernel..3=user)
    Bit 4: Set to 0 for interrupt gates
    Bits 3-0: bits 1110 = decimal 15 = "32 bit interrupt gate"
    */
    unsigned char flags;
    unsigned short high_offset; // Higher 16 bits of handler function address
} __attribute__((packed)) idt_gate_t;

// A pointer to the array of interrupt handlers, will be read by asm 'lidt'
typedef struct
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
extern idt_gate_t idt[IDT_ENTRIES];
extern idt_register_t idt_register;

void set_idt_gate(int n, unsigned int handler);
void set_idt();

#endif