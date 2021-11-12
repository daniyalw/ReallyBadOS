#include "idt.h"

void set_idt_gate(int n, uint32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = 0x08;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (u32) &idt;
    idt_reg.limit = 256 * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
    __asm__ __volatile__("sti"); /* enable interrupts */
}
