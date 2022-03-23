#include <sys/descriptors/idt.h>

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

namespace Kernel {

namespace CPU {

void set_idt_gate(int n, uint32 handler) {
    Kernel::CPU::idt[n].low_offset = low_16(handler);
    Kernel::CPU::idt[n].sel = 0x08;
    Kernel::CPU::idt[n].always0 = 0;
    Kernel::CPU::idt[n].flags = 0x8E | 0x60;
    Kernel::CPU::idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (uint32_t) & idt;
    idt_reg.limit = 256 * sizeof(Kernel::CPU::idt_gate_t) - 1;
    asm volatile("lidtl (%0)" : : "r" (&Kernel::CPU::idt_reg));
    asm volatile("sti");
}

}

}
