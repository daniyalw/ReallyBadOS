#pragma once
#include <sys/time/time.h>
#include <sys/time/timer.h>
#include <sys/io.h>
#include <sys/log/log.h>
#include <sys/multitasking/task.h>

int task_counter = 0;
const int task_counter_limit = 200;

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

    if (tasking_on)
    {
        task_counter++;

        if (task_counter == task_counter_limit)
        {
            task_counter = 0;
            //switch_task(regs);
        }
    }

    /*
    for (int z = 0; z < timers.size(); z++) {
        timer_t timer = timers[z];
        timer.remaining_ms--;
        timers.replace(timer, z);
    }

    for (int z = 0; z < timers.size(); z++) {
        timer_t timer = timers[z];

        if (timer.remaining_ms == 0) {
            timers.remove(z);
            timer.func();
        }
    }
    */
}

namespace Kernel {

void init_timer(uint frequency) {
    log::info("Timer enabled.");
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

void async_timer(int ms, void (*function)())
{
    timer_t timer;
    timer.func = function;
    timer.remaining_ms = ms;
    timers.push_back(timer);
}
