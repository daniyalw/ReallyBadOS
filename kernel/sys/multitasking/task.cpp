#include "task.h"

void idle_task()
{
    while (true) {}
}

void efddsfds()
{
    int z = 0;

    while (true)
    {
    }
}

void mdasd()
{
    printf("EEEEEE");
    exit(0);
}

void yd()
{
    printf("Yielding");
    while (true)
    {
        yield();
    }
}

void nmc()
{
    int code = wait_retcode(5);

    printf("Code: %d\n", code);

    exit(0);
}

void abc()
{
    exit(4);
}

void block_task(task_t *task)
{
    task->blocked = true;
    tasks[task->pid] = task;
}

void block_task_id(int taskid)
{
    ASSERT(taskid >= 0 && taskid < task_count);
    task_t *task = tasks[taskid];
    block_task(task);
}

void unblock_task(task_t *task)
{
    task->blocked = false;
    tasks[task->pid] = task;
}

void unblock_task_id(int taskid)
{
    ASSERT(taskid >= 0 && taskid < task_count);
    task_t *task = tasks[taskid];
    unblock_task(task);
}

int find_retcode(pid_t pid)
{
    ASSERT(pid >= 0 && pid < task_count);

    if (tasks[pid]->null)
        return tasks[pid]->ret;
    else
        return -1;
}

int wait_retcode(pid_t pid)
{
    ASSERT(pid >= 0 && pid < task_count);

    while (true)
    {
        if (tasks[pid]->null)
            return tasks[pid]->ret;
        else
            yield();
    }
}

void exit(int ret)
{
    task_t *task = tasks[current_task];

    if (task == NULL)
        return;

    task->ret = ret;
    // we can't free the task because we save task->ret
    free((void *)task->stack);
    task->null = true;

    task_counter = 0;
    switch_task(NULL, false);
}

void yield()
{
    task_counter = 0;
    switch_task(NULL, true);
}

void create_process(char *name, uint32_t begin)
{
    task_t *task = (task_t *)malloc(sizeof(task_t *));

    memset(task->name, 0, 20);
    strcpy(task->name, name);

    task->pid = task_count;

    task->eip = begin;
    task->eflags = 0x202;
    task->null = false;
    task->blocked = false;

    uint32_t stack_addr = allocate_stack();
    uint32_t *stack = (uint32_t *)stack_addr + (4 * 1024);

    *--stack = task->eflags; // eflags
    *--stack = 0x0b; // cs
    *--stack = (uint32_t)begin; // eip
    *--stack = 0; // eax
    *--stack = 0; // ebx
    *--stack = 0; // ecx;
    *--stack = 0; //edx
    *--stack = 0; //esi
    *--stack = 0; //edi
    *--stack = stack_addr + (4 * 1024); //ebp
    *--stack = 0x13; // ds
    *--stack = 0x13; // fs
    *--stack = 0x13; // es
    *--stack = 0x13; // gs

    task->stack_top = (uint32_t)stack;
    task->esp = task->stack_top;
    task->stack = stack_addr;

    tasks[task_count] = task;
    task_count++;
}

void load_new_task(task_t *task)
{
    uint32_t esp, eip, ebp;
    esp = task->esp;
    eip = task->eip;
    ebp = task->ebp;

    perform_task_switch(eip, ebp, esp);
}

uint32_t allocate_stack()
{
    return malloc(4 * 1024);
}

void switch_task(registers_t *regs, bool save)
{
    uint32_t esp, ebp;
    asm volatile ("mov %%esp, %0" : "=r" (esp));
	asm volatile ("mov %%ebp, %0" : "=r" (ebp));
    uint32_t eip = read_eip();

    if (eip == 0x12344)
        return;

    task_t *current = tasks[current_task];

    if (save)
    {
        current->eip = eip;
        current->esp = esp;
        current->ebp = ebp;

        current->stack_top = current->esp;

        tasks[current->pid] = current;
    }

    current_task++;

    if (current_task >= task_count)
        current_task = 0;

    while (tasks[current_task]->null || tasks[current_task]->blocked)
    {
        current_task++;

        if (current_task >= task_count)
            current_task = 0;
    }

    task_t *load = tasks[current_task];
#ifdef DEBUG
    log::warning("Current task: %d\nNew task load: %s\nNew task eip: 0x%x\n", current_task, load->name, load->eip);
#endif
    load_new_task(load);
}

void init_tasking()
{
    create_process("idle", (uint32_t)&idle_task);
    create_process("test", (uint32_t)&efddsfds);
    create_process("md", (uint32_t)&mdasd);
    create_process("yd", (uint32_t)&yd);
    create_process("nmc", (uint32_t)&nmc);
    create_process("abc", (uint32_t)&abc);

    tasking_on = true;
    switch_task(NULL, false);
}
