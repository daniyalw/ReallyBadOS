#pragma once

extern "C"
{
    extern void perform_task_switch(uint32_t, uint32_t, uint32_t);
    extern uint32_t read_eip();
}

typedef struct
{
    char name[20];
    int eip, esp, ebp, eflags;
    int pid;
    uint32_t stack;
    uint32_t stack_top;
    bool null, blocked;
} task_t;

task_t *tasks[30];
int current_task = 0;
int task_count = 0;

int tasking_on = false;

void create_process(char *name, uint32_t begin);
void load_new_task(task_t *task);

void save_task_context(task_t *task, registers_t *regs);

uint32_t allocate_stack();

void init_tasking();
void switch_task(registers_t *regs, bool save);
