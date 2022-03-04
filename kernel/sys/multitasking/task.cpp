#include <sys/descriptors/isr.h>
#include <sys/multitasking/task.h>
#include <sys/descriptors/gdt.h>

void idle_task()
{
    while (true)
    {

    }
}

void yield()
{
    task_counter = 0; // the timer will switch to a new task when it sees that the counter is 0
}

void exit(int ret)
{
    task_t *task = (task_t *)&tasks[current_task];
    task->null = true;
    free((void *)task->stack);

    yield();
}

task_t *find_first_null_task()
{
    for (int z = 0; z < task_count; z++)
    {
        if (tasks[z].null)
            return (task_t *)&tasks[z];
    }

    return NULL;
}

task_t *first_free_task()
{
    task_t *task = find_first_null_task();

    if (task == NULL)
        task = (task_t *)&tasks[task_count];

    return task;
}

void block_task(int task_id)
{
    if (task_id >= task_count || task_id < 0)
        return;

    task_t *task = (task_t *)&tasks[task_id];

    if (!task->null)
    {
        task->blocked = true;
    }
}

void unblock_task(int task_id)
{
    if (task_id >= task_count || task_id < 0)
        return;

    task_t *task = (task_t *)&tasks[task_id];

    if (!task->null)
    {
        task->blocked = false;
    }
}

void block_current_task()
{
    block_task(current_task);
}

void unblock_current_task()
{
    unblock_task(current_task);
}

void create_process(char *name, uint32_t begin)
{
    task_t *task = first_free_task();

    strcpy(task->name, name);
    task->pid = task_count;
    task_count++;

    task->eip = begin;
    task->eflags = 0x202;
    task->null = false;

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

    uint32_t stack_bottom = current->stack;
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

    current->stack = stack_bottom;
    current->stack_top = (uint32_t)stack;
    current->esp = current->stack_top;

    int current = current_task;

    current_task++;

    if (current_task >= task_count)
        current_task = 0;

    while (tasks[current_task].null || tasks[current_task].blocked)
    {
        current_task++;

        if (current_task >= task_count)
            current_task = 0;

        if (current_task == current)
        {
            // one full circle, all tasks blocked or null
            PANIC("All tasks, including idle, blocked or null!");
        }
    }

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
