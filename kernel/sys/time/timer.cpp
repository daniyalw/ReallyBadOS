#pragma once
#include "../interrupts/isr.h"
#include "time.h"
#include "timer.h"
#include "../io.cpp"

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

void init_timer(u32 freq) {
    system_log("Enabled timer.\n");
    hz = freq;
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);
    /* Send the command */
    outb(0x43, 0x36); /* Command port */
    outb(0x40, low);
    outb(0x40, high);
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
