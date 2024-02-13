#include "isr.h"

#include "idt.h"
#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../util/console.h"

isr_t interrupt_handlers[256];

// Can't loop because address of function names needed
void isr_install()
{
    set_idt_gate(0, (unsigned int)isr0);
    set_idt_gate(1, (unsigned int)isr1);
    set_idt_gate(2, (unsigned int)isr2);
    set_idt_gate(3, (unsigned int)isr3);
    set_idt_gate(4, (unsigned int)isr4);
    set_idt_gate(5, (unsigned int)isr5);
    set_idt_gate(6, (unsigned int)isr6);
    set_idt_gate(7, (unsigned int)isr7);
    set_idt_gate(8, (unsigned int)isr8);
    set_idt_gate(9, (unsigned int)isr9);
    set_idt_gate(10, (unsigned int)isr10);
    set_idt_gate(11, (unsigned int)isr11);
    set_idt_gate(12, (unsigned int)isr12);
    set_idt_gate(13, (unsigned int)isr13);
    set_idt_gate(14, (unsigned int)isr14);
    set_idt_gate(15, (unsigned int)isr15);
    set_idt_gate(16, (unsigned int)isr16);
    set_idt_gate(17, (unsigned int)isr17);
    set_idt_gate(18, (unsigned int)isr18);
    set_idt_gate(19, (unsigned int)isr19);
    set_idt_gate(20, (unsigned int)isr20);
    set_idt_gate(21, (unsigned int)isr21);
    set_idt_gate(22, (unsigned int)isr22);
    set_idt_gate(23, (unsigned int)isr23);
    set_idt_gate(24, (unsigned int)isr24);
    set_idt_gate(25, (unsigned int)isr25);
    set_idt_gate(26, (unsigned int)isr26);
    set_idt_gate(27, (unsigned int)isr27);
    set_idt_gate(28, (unsigned int)isr28);
    set_idt_gate(29, (unsigned int)isr29);
    set_idt_gate(30, (unsigned int)isr30);
    set_idt_gate(31, (unsigned int)isr31);

    // Remap the PIC
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    // Install the IRQs
    set_idt_gate(32, (unsigned int)irq0);
    set_idt_gate(33, (unsigned int)irq1);
    set_idt_gate(34, (unsigned int)irq2);
    set_idt_gate(35, (unsigned int)irq3);
    set_idt_gate(36, (unsigned int)irq4);
    set_idt_gate(37, (unsigned int)irq5);
    set_idt_gate(38, (unsigned int)irq6);
    set_idt_gate(39, (unsigned int)irq7);
    set_idt_gate(40, (unsigned int)irq8);
    set_idt_gate(41, (unsigned int)irq9);
    set_idt_gate(42, (unsigned int)irq10);
    set_idt_gate(43, (unsigned int)irq11);
    set_idt_gate(44, (unsigned int)irq12);
    set_idt_gate(45, (unsigned int)irq13);
    set_idt_gate(46, (unsigned int)irq14);
    set_idt_gate(47, (unsigned int)irq15);

    set_idt(); // Load with ASM
}

void print_exception(unsigned int interrupt_number)
{
    switch (interrupt_number)
    {
    case 0:
        print_color("Division By Zero\n", 0x28);
        break;
    case 1:
        print_color("Debug\n", 0x2A);
        break;
    case 2:
        print_color("Non Maskable Interrupt\n", 0x2A);
        break;
    case 3:
        print_color("Breakpoint\n", 0x2A);
        break;
    case 4:
        print_color("Into Detected Overflow\n", 0x2A);
        break;
    case 5:
        print_color("Out of Bounds\n", 0x28);
        break;
    case 6:
        print_color("Invalid Opcode\n", 0x28);
        break;
    case 7:
        print_color("No Coprocessor\n", 0x28);
        break;
    case 8:
        print_color("Double Fault\n", 0x28);
        break;
    case 9:
        print_color("Coprocessor Segment Overrun\n", 0x28);
        break;
    case 10:
        print_color("Bad TSS\n", 0x28);
        break;
    case 11:
        print_color("Segment Not Present\n", 0x28);
        break;
    case 12:
        print_color("Stack Fault\n", 0x28);
        break;
    case 13:
        print_color("General Protection Fault\n", 0x28);
        break;
    case 14:
        print_color("Page Fault\n", 0x28);
        break;
    case 15:
        print_color("Unknown Interrupt\n", 0x2A);
        break;
    case 16:
        print_color("Coprocessor Fault\n", 0x28);
        break;
    case 17:
        print_color("Alignment Check\n", 0x2A);
        break;
    case 18:
        print_color("Machine Check\n", 0x2A);
        break;
    // Cases 19 to 31 are "Reserved"
    default:
        if (interrupt_number >= 19 && interrupt_number <= 31)
        {
            print_color("Reserved Interrupt\n", 0x2A);
        }
        else
        {
            print_color("Unknown Interrupt Number\n", 0x2A);
        }
        break;
    }
    draw_console();
    flip();
    while (true)
    {
    }
}

void isr_handler(registers_t registers)
{
    print_exception(registers.interrupt_number);
}

void register_interrupt_handler(unsigned char n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t registers)
{
    // After every interrupt send an EOI to the PICs
    if (registers.interrupt_number >= 40)
    {
        port_byte_out(0xA0, 0x20); // Slave
    }
    port_byte_out(0x20, 0x20); // Master

    // Handle the interrupt in a more modular way
    if (interrupt_handlers[registers.interrupt_number] != 0)
    {
        isr_t handler = interrupt_handlers[registers.interrupt_number];
        handler(registers);
    }
}