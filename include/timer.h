#pragma once
#include <kernel/isr.h>

u32 tick = 0;
int seconds = 0;
u32 ctick;
u32 hz;
static void timer_callback(registers_t regs);
void init_timer(u32 freq);
void timer_wait(int ticks);
void sleep(int secs);
