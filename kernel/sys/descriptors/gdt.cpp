#pragma once
#include <sys/descriptors/gdt.h>
#include <sys/log/log.h>

namespace Kernel {

    extern "C" void user_func()
    {
    }

void _init_tss(uint32_t i, uint16_t kernel_ss, uint16_t kernel_esp)
{
   gdt_set_gate(i, (unsigned int)&tss, (unsigned int)&tss + sizeof(TSS) - 1, 0x89, 0x00);

   tss.ss0 = kernel_ss;
   tss.esp0 = kernel_esp;

   tss.cs = 0x0b;
   tss.ss = 0x13;
   tss.es = 0x13;
   tss.ds = 0x13;
   tss.fs = 0x13;
   tss.gs = 0x13;

   log::info("Initialized TSS.");

   flush_tss();
}

void init_tss()
{
    uint32_t esp;
    asm volatile("mov %%esp, %0" : "=r"(esp));
    Kernel::_init_tss(5, 0x10, esp);
}

static void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
   gdt_ptr.base  = (u32)&gdt_entries;

   Kernel::gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   Kernel::gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   Kernel::gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   Kernel::gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   Kernel::gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   gdt_flush((u32)&gdt_ptr);
   log::info("Enabled GDT.");
}

static void gdt_set_gate(s32 num, u32 base, u32 limit, u8 access, u8 gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}

}
