#pragma once
#include "idt.h"

void idt_set(int n, uint32 handler) {
    idt[n].l_offset = low_16(handler);
    idt[n].sel = 0x08;
    idt[n].always_zero = 0;
    idt[n].flags = 0x8E;
    idt[n].h_offset = high_16(handler);
}

void set_idt() {
    idt_register.base = (u32) &idt;
    idt_register.limit = 256 * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_register));
    __asm__ __volatile__("sti"); /* enable interrupts */
}
