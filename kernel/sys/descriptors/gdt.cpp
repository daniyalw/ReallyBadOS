#pragma once
#include <sys/descriptors/gdt.h>
#include <sys/log/log.h>

namespace Kernel {

namespace CPU {

void _init_tss(uint32_t i, uint16_t kernel_ss, uint16_t kernel_esp)
{
    gdt_set_gate(i, (unsigned int)&tss, (unsigned int)&tss + sizeof(TSS) - 1, 0x89, 0x00);

    Kernel::CPU::tss.ss0 = kernel_ss;
    Kernel::CPU::tss.esp0 = kernel_esp;

    Kernel::CPU::tss.cs = 0x0b;
    Kernel::CPU::tss.ss = 0x13;
    Kernel::CPU::tss.es = 0x13;
    Kernel::CPU::tss.ds = 0x13;
    Kernel::CPU::tss.fs = 0x13;
    Kernel::CPU::tss.gs = 0x13;

    log::info("Initialized TSS.");

    flush_tss();
}

void init_tss()
{
    uint32_t esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    Kernel::CPU::_init_tss(5, 0x10, esp);
}

void set_kernel_stack(uint32_t stack)
{
    tss.esp0 = stack;
    flush_tss();
}

static void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
    gdt_ptr.base  = (u32)&gdt_entries;

    Kernel::CPU::gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    Kernel::CPU::gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    Kernel::CPU::gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    Kernel::CPU::gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    Kernel::CPU::gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    gdt_flush((u32)&gdt_ptr);
    log::info("Enabled GDT.");
}

static void gdt_set_gate(s32 num, u32 base, u32 limit, u8 access, u8 gran)
{
    Kernel::CPU::gdt_entries[num].base_low    = (base & 0xFFFF);
    Kernel::CPU::gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    Kernel::CPU::gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    Kernel::CPU::gdt_entries[num].limit_low   = (limit & 0xFFFF);
    Kernel::CPU::gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    Kernel::CPU::gdt_entries[num].granularity |= gran & 0xF0;
    Kernel::CPU::gdt_entries[num].access      = access;
}

}

}
