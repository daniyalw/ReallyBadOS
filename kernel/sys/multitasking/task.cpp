#include <sys/descriptors/isr.h>
#include <sys/multitasking/task.h>
#include <sys/descriptors/gdt.h>

void create_process(char *name, uint32_t begin)
{
    task_t *task = (task_t *)&tasks[task_count];

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

void switch_task(registers_t *regs)
{
    uint32_t eip = regs->eip;

    if (eip == 0x12344)
        return;

    task_t *current = (task_t *)&tasks[current_task];

    uint32_t stack_bottom = allocate_stack();
    uint32_t *stack = (uint32_t *)stack_bottom + 4096;

    *--stack = current->eflags; // eflags
    *--stack = 0x0b; // cs
    *--stack = current->eip; // eip
    *--stack = 0; // eax
    *--stack = 0; // ebx
    *--stack = 0; // ecx;
    *--stack = 0; //edx
    *--stack = 0; //esi
    *--stack = 0; //edi
    *--stack = stack_bottom + (4 * 1024); //ebp
    *--stack = 0x13; // ds
    *--stack = 0x13; // fs
    *--stack = 0x13; // es
    *--stack = 0x13; // gs

#ifdef DEBUG
    log::info("Note: eip: %d\n", current->eip);
#endif

    free(current->stack);

    current->stack = stack_bottom;
    current->stack_top = (uint32_t)stack;
    current->esp = current->stack_top;

    current_task++;

    if (current_task >= task_count)
        current_task = 0;

    task_t *load = (task_t *)&tasks[current_task];
#ifdef DEBUG
    log::warning("Current task: %d\nNew task load: %s\nNew task eip: %d\ntest2(): %d\n", current_task, load->name, load->eip, (uint32_t)&test3);
#endif
    load_new_task(load);
}

void init_tasking()
{
    create_process("idle", (uint32_t)&idle_task);

    tasking_on = true;
}
