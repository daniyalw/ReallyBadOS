#pragma once

typedef struct {
    uint16 l_offset;
    uint16 sel;
    uint8 always_zero;
    uint8 flags;
    uint16 h_offset;
} __attribute__((packed)) idt_gate_t ;

typedef struct {
    uint16 limit;
    uint32 base;
} __attribute__((packed)) idt_register_t;

idt_gate_t idt[256];
idt_register_t idt_register;
