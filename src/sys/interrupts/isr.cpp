#pragma once
#include "../../include/string.cpp"
#include "../../drivers/video/video.cpp"
#include "idt.h"
#include "idt.cpp"
#include "isr.h"

isr_t interrupt_handlers[256];

void isr_install() {
    idt_set(0, (u32)isr0);
    idt_set(1, (u32)isr1);
    idt_set(2, (u32)isr2);
    idt_set(3, (u32)isr3);
    idt_set(4, (u32)isr4);
    idt_set(5, (u32)isr5);
    idt_set(6, (u32)isr6);
    idt_set(7, (u32)isr7);
    idt_set(8, (u32)isr8);
    idt_set(9, (u32)isr9);
    idt_set(10, (u32)isr10);
    idt_set(11, (u32)isr11);
    idt_set(12, (u32)isr12);
    idt_set(13, (u32)isr13);
    idt_set(14, (u32)isr14);
    idt_set(15, (u32)isr15);
    idt_set(16, (u32)isr16);
    idt_set(17, (u32)isr17);
    idt_set(18, (u32)isr18);
    idt_set(19, (u32)isr19);
    idt_set(20, (u32)isr20);
    idt_set(21, (u32)isr21);
    idt_set(22, (u32)isr22);
    idt_set(23, (u32)isr23);
    idt_set(24, (u32)isr24);
    idt_set(25, (u32)isr25);
    idt_set(26, (u32)isr26);
    idt_set(27, (u32)isr27);
    idt_set(28, (u32)isr28);
    idt_set(29, (u32)isr29);
    idt_set(30, (u32)isr30);
    idt_set(31, (u32)isr31);

    // Remap the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // Install the IRQs
    idt_set(32, (u32)irq0);
    idt_set(33, (u32)irq1);
    idt_set(34, (u32)irq2);
    idt_set(35, (u32)irq3);
    idt_set(36, (u32)irq4);
    idt_set(37, (u32)irq5);
    idt_set(38, (u32)irq6);
    idt_set(39, (u32)irq7);
    idt_set(40, (u32)irq8);
    idt_set(41, (u32)irq9);
    idt_set(42, (u32)irq10);
    idt_set(43, (u32)irq11);
    idt_set(44, (u32)irq12);
    idt_set(45, (u32)irq13);
    idt_set(46, (u32)irq14);
    idt_set(47, (u32)irq15);

    set_idt(); // Load with ASM
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

extern "C" void isr_handler(registers_t r) {
    if (r.int_no == 6)  {
        return;
    }

    printf("received interrupt: ");
    char s[3];
    itoa(r.int_no, s);
    printf(s);
    printf("\n");
    printf(exception_messages[r.int_no]);
    printf("\n");
}

void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

extern "C" void irq_handler(registers_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r.int_no >= 40) outb(0xA0, 0x20); /* slave */
    outb(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}
