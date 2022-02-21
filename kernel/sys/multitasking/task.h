#pragma once

typedef struct context_t {
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t eflags;
    uint32_t cr3;
    uint32_t eip;
} context_t;

extern "C"
{
    extern void kernel_regs_switch(uint32_t, uint32_t, uint32_t);
}

typedef struct
{
    char name[20];
    context_t regs;
    int pid;
    uint32_t stack;
} task_t;

task_t tasks[10];
int current_task = 0;
int task_count = 0;

int tasking_on = false;

uint32_t beginning_stack = 0xC0000000;

void create_process(char *name, uint32_t begin);
void load_new_task(task_t task);

void save_task(task_t task);
void save_task_context(task_t task, registers_t regs);

uint32_t allocate_stack();

void init_tasking();
void switch_task(registers_t regs);
