#pragma once
#include <sys/time/time.h>
#include <sys/time/timer.h>
#include <sys/io.h>
#include <sys/log/log.h>
#include <sys/multitasking/task.h>
#include <sys/multitasking/thread.h>

void timer_int(registers_t *regs) {
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

    if (hour >= 12) {
        if (hour > 12) hour -= 12;
        pm_on = true;
    }

    if (Kernel::CPU::tasking_on) {
        task_counter++;

        if (task_counter == task_counter_limit) {
            task_counter = 0;
            Kernel::CPU::switch_task(regs, true);
        }
    }
}

namespace Kernel {

void init_timer(uint frequency) {
    log::info("Timer enabled.");
    hz = frequency;

    Kernel::CPU::register_interrupt_handler(IRQ0, timer_int);

    uint divisor = 1193180/frequency;

    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    Kernel::IO::outb(0x43, 0x36);
    Kernel::IO::outb(0x40, low);
    Kernel::IO::outb(0x40, high);
}

}

void timer_wait(int ticks) {
    unsigned long eticks;

    eticks = tick + ticks;
    while(tick < eticks);
}

void sleep(int ms) {
    timer_wait(ms);
}

void sleep_sec(int sec) {
    timer_wait(sec * hz);
}

inline int get_uptime() {
    return tick;
}
