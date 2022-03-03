#pragma once

// http://jamesmolloy.co.uk/tutorial_html/

typedef struct
{
   u16 limit_low;           // The lower 16 bits of the limit.
   u16 base_low;            // The lower 16 bits of the base.
   u8  base_middle;         // The next 8 bits of the base.
   u8  access;              // Access flags, determine what ring this segment can be used in.
   u8  granularity;
   u8  base_high;           // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_t;


typedef struct
{
   u16 limit;               // The upper 16 bits of all selector limits.
   u32 base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed)) gdt_ptr_t;

extern "C" {
    extern void gdt_flush(u32 push);
    extern void jump_usermode();
}

namespace Kernel {
// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(s32 num, u32 base, u32 limit, u8 access, u8 gran);
void _init_tss(uint32_t i, uint16_t kernel_ss, uint16_t kernel_esp);
void init_tss();
void set_kernel_stack(uint32_t stack);

}

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

struct __attribute__((packed)) TSS
{
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
};


static TSS tss = {
    .prev_tss = 0,
    .esp0 = 0,
    .ss0 = 0x10,
    .esp1 = 0,
    .ss1 = 0,
    .esp2 = 0,
    .ss2 = 0,
    .cr3 = 0,
    .eip = 0,
    .eflags = 0x0202,
    .eax = 0,
    .ecx = 0,
    .edx = 0,
    .ebx = 0,
    .esp = 0,
    .ebp = 0,
    .esi = 0,
    .edi = 0,
    .es = 0,
    .cs = 0,
    .ss = 0,
    .ds = 0,
    .fs = 0,
    .gs = 0,
    .ldt = 0,
    .trap = 0,
    .iomap_base = 0,
};

extern "C" {
    extern void flush_tss();
}
