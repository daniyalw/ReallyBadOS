#include <sys/descriptors/isr.h>
#include <sys/multitasking/task.h>

void create_process(char *name, uint32_t begin)
{
    task_t task;

    strcpy(task.name, name);

    task.pid = task_count;
    task_count++;

    task.regs.eip = begin;
    task.regs.eflags = 0x206;;

    task.stack = allocate_stack();
    task.regs.esp = (task.stack - 4 * 1024);

    save_task(task);
}

void load_new_task(task_t task)
{
   asm volatile("iret");
}

uint32_t allocate_stack()
{
    uint32_t stack = beginning_stack;
    beginning_stack += 4 * 1024;
    return stack;
}

void save_task(task_t task)
{
    tasks[task.pid] = task;
}

void save_task_context(task_t task, registers_t regs)
{
    task.regs.eax = regs.eax;
    task.regs.ebx = regs.ebx;
    task.regs.ecx = regs.ecx;
    task.regs.edx = regs.edx;
    task.regs.esp = regs.esp;
    task.regs.ebp = regs.ebp;
    task.regs.esi = regs.esi;
    task.regs.edi = regs.edi;
    task.regs.eflags = regs.eflags;
    task.regs.eip = regs.eip;
    save_task(task);
}

void switch_task(registers_t regs)
{
    task_t current = tasks[current_task];
    save_task_context(current, regs);

    if (current_task - 1 >= task_count)
        current_task = 0;

    task_t load = tasks[current_task];
    load_new_task(load);
}

void test1()
{
    while (true)
    {
        printf("1");
    }
}

void test2()
{
    while (true)
    {
        printf("2");
    }
}

void test3()
{
    while (true)
    {
        printf("3");
    }
}

void init_tasking()
{
    create_process("test1", (uint32_t)&test1);
    create_process("test2", (uint32_t)&test2);
    create_process("test3", (uint32_t)&test3);

    tasking_on = true;
}
