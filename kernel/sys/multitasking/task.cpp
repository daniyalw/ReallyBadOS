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
        printf("Z:%d\n", z++);
    }
}

void create_process(char *name, uint32_t begin)
{
    task_t *task = (task_t *)malloc(sizeof(task_t *));

    memset(task->name, 0, 20);
    strcpy(task->name, name);

    task->pid = task_count;
    task_count++;

    task->eip = begin;
    task->eflags = 0x202;

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

    asm volatile (
			"mov %0, %%ebx\n"
			"mov %1, %%esp\n"
			"mov %2, %%ebp\n"
			"mov $0x12344, %%eax\n"
			"jmp *%%ebx"
			: : "r" (eip), "r" (esp), "r" (ebp)
			: "%ebx", "%esp", "%eax");
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

    tasking_on = true;
    switch_task(NULL, false);
}
