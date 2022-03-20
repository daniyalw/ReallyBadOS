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
    int ret;
} task_t;

task_t *tasks[30];
int current_task = 0;
int task_count = 0;

int tasking_on = false;

typedef int pid_t;

void create_process(char *name, uint32_t begin);
void load_new_task(task_t *task);

void save_task_context(task_t *task, registers_t *regs);

uint32_t allocate_stack();

void init_tasking();
void switch_task(registers_t *regs, bool save);

void exit(int ret);
void yield();

void block_task(task_t *task);
void block_task_id(int taskid);
void unblock_task(task_t *task);
void unblock_task_id(int taskid);

int wait_retcode(pid_t pid);
