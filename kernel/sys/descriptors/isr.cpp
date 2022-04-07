#include <string.h>
#include <video/video.h>
#include <sys/descriptors/isr.h>
#include <sys/log/log.h>
#include <sys/multitasking/task.h>

namespace Kernel {

namespace CPU {

/*
BSD 3-Clause License

Copyright (c) 2018, Carlos Fenollosa
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

isr_t interrupt_handlers[256];

void pic_remap() {
    Kernel::IO::outb(0x20, 0x11);
    Kernel::IO::outb(0xA0, 0x11);
    Kernel::IO::outb(0x21, 0x20);
    Kernel::IO::outb(0xA1, 0x28);
    Kernel::IO::outb(0x21, 0x04);
    Kernel::IO::outb(0xA1, 0x02);
    Kernel::IO::outb(0x21, 0x01);
    Kernel::IO::outb(0xA1, 0x01);
    Kernel::IO::outb(0x21, 0x0);
    Kernel::IO::outb(0xA1, 0x0);
}

void init_isr() {
    Kernel::CPU::set_idt_gate(0, (u32)isr0);
    Kernel::CPU::set_idt_gate(1, (u32)isr1);
    Kernel::CPU::set_idt_gate(2, (u32)isr2);
    Kernel::CPU::set_idt_gate(3, (u32)isr3);
    Kernel::CPU::set_idt_gate(4, (u32)isr4);
    Kernel::CPU::set_idt_gate(5, (u32)isr5);
    Kernel::CPU::set_idt_gate(6, (u32)isr6);
    Kernel::CPU::set_idt_gate(7, (u32)isr7);
    Kernel::CPU::set_idt_gate(8, (u32)isr8);
    Kernel::CPU::set_idt_gate(9, (u32)isr9);
    Kernel::CPU::set_idt_gate(10, (u32)isr10);
    Kernel::CPU::set_idt_gate(11, (u32)isr11);
    Kernel::CPU::set_idt_gate(12, (u32)isr12);
    Kernel::CPU::set_idt_gate(13, (u32)isr13);
    Kernel::CPU::set_idt_gate(14, (u32)isr14);
    Kernel::CPU::set_idt_gate(15, (u32)isr15);
    Kernel::CPU::set_idt_gate(16, (u32)isr16);
    Kernel::CPU::set_idt_gate(17, (u32)isr17);
    Kernel::CPU::set_idt_gate(18, (u32)isr18);
    Kernel::CPU::set_idt_gate(19, (u32)isr19);
    Kernel::CPU::set_idt_gate(20, (u32)isr20);
    Kernel::CPU::set_idt_gate(21, (u32)isr21);
    Kernel::CPU::set_idt_gate(22, (u32)isr22);
    Kernel::CPU::set_idt_gate(23, (u32)isr23);
    Kernel::CPU::set_idt_gate(24, (u32)isr24);
    Kernel::CPU::set_idt_gate(25, (u32)isr25);
    Kernel::CPU::set_idt_gate(26, (u32)isr26);
    Kernel::CPU::set_idt_gate(27, (u32)isr27);
    Kernel::CPU::set_idt_gate(28, (u32)isr28);
    Kernel::CPU::set_idt_gate(29, (u32)isr29);
    Kernel::CPU::set_idt_gate(30, (u32)isr30);
    Kernel::CPU::set_idt_gate(31, (u32)isr31);

    pic_remap();

    Kernel::CPU::set_idt_gate(32, (u32)irq0);
    Kernel::CPU::set_idt_gate(33, (u32)irq1);
    Kernel::CPU::set_idt_gate(34, (u32)irq2);
    Kernel::CPU::set_idt_gate(35, (u32)irq3);
    Kernel::CPU::set_idt_gate(36, (u32)irq4);
    Kernel::CPU::set_idt_gate(37, (u32)irq5);
    Kernel::CPU::set_idt_gate(38, (u32)irq6);
    Kernel::CPU::set_idt_gate(39, (u32)irq7);
    Kernel::CPU::set_idt_gate(40, (u32)irq8);
    Kernel::CPU::set_idt_gate(41, (u32)irq9);
    Kernel::CPU::set_idt_gate(42, (u32)irq10);
    Kernel::CPU::set_idt_gate(43, (u32)irq11);
    Kernel::CPU::set_idt_gate(44, (u32)irq12);
    Kernel::CPU::set_idt_gate(45, (u32)irq13);
    Kernel::CPU::set_idt_gate(46, (u32)irq14);
    Kernel::CPU::set_idt_gate(47, (u32)irq15);
    Kernel::CPU::set_idt_gate(48, (u32)irq16);
    Kernel::CPU::set_idt_gate(49, (u32)irq17);

    Kernel::CPU::set_idt();

    log::info("Enabled interrupts.");
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

    if (tasking_on) {
        exit(1);
    }

    if (interrupt_handlers[r.int_no] != NULL) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(&r);
    } else {
        log::error("Interrupt received: %d\nMessage: %s", r.int_no, exception_messages[r.int_no]);
    }

    log::warning("Interrupt received");

    asm("cli");
    asm("hlt");
}

void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

extern "C" void irq_handler(registers_t r) {
    if (r.int_no >= 40) Kernel::IO::outb(0xA0, 0x20);
    Kernel::IO::outb(0x20, 0x20);

    //Kernel::system_log("IRQ received\n");

    if (interrupt_handlers[r.int_no] != NULL) {
        isr_t handler = Kernel::CPU::interrupt_handlers[r.int_no];
        handler(&r);
    }
    else
    {
        log::error("unknown IRQ: %d", r.int_no);
    }
}

}

}
