#pragma once
#include <time.h>
#include <timer.h>
#include <kernel/io.h>
#include <kernel/log.h>

void timer_int(registers_t regs) {
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

void init_timer(uint frequency) {
    Kernel::system_log("Timer enabled.\n");
    hz = frequency;

    Kernel::register_interrupt_handler(IRQ0, timer_int);

    uint divisor = 1193180/frequency;

    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x43, 0x36);
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
