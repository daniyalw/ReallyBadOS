#pragma once
#include <time.h>
#include <timer.h>
#include <kernel/io.h>
#include <kernel/log.h>

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

static void timer_callback(registers_t regs) {
    tick++;
    ctick++;

    if (ctick == hz) {
        ctick = 0;
        seconds++;
    }

    if (seconds == 60) {
        seconds = 0;
        minute++;
    }

    if (minute == 60) {
        minute = 0;
        hour++;
    }

    if (hour > 12) {
        hour -= 12;
        pm_on = true;
    }
}

namespace Kernel {

void init_timer(u32 freq) {
    Kernel::system_log("Enabled timer.\n");
    hz = freq;
    /* Install the function we just wrote */
    Kernel::register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);
    /* Send the command */
    outb(0x43, 0x36); /* Command port */
    outb(0x40, low);
    outb(0x40, high);
}

}

void timer_wait(int ticks)
{
    unsigned long eticks;

    eticks = tick + ticks;
    while(tick < eticks);
}

void sleep(int secs)
{
    timer_wait(secs * hz);
}

void sleep_ms(int ms)
{
    timer_wait(ms);
}
