
typedef struct {
    uint16 low_offset;
    uint16 sel;
    uint8 always0;
    uint8 flags;
    uint16 high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16 limit;
    uint32 base;
} __attribute__((packed)) idt_register_t;

idt_gate_t idt[256];
idt_register_t idt_reg;
