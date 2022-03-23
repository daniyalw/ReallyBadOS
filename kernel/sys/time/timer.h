#pragma once
#include <sys/descriptors/isr.h>

u32 tick = 0;
int seconds = 0;
u32 ctick;
u32 hz;

int task_counter = 0;
const int task_counter_limit = 500;

static void timer_callback(registers_t regs);
void timer_wait(int ticks);
void sleep(int secs);
