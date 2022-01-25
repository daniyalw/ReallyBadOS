#pragma once
#include <sys/descriptors/isr.h>

u32 tick = 0;
int seconds = 0;
u32 ctick;
u32 hz;

typedef struct
{
    int remaining_ms;
    void (*func)(); // function to run when timer is up
} timer_t;

std::list<timer_t> timers;

static void timer_callback(registers_t regs);
void timer_wait(int ticks);
void sleep(int secs);
void async_timer(int ms, void (*function)());
