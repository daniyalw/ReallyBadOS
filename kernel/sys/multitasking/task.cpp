#include "task.h"

int idle_task()
{
    while (true) {}
    return 0;
}

int efddsfds()
{
    int z = 0;

    while (true)
    {
    }
    return 0;
}

int mdasd()
{
    printf("EEEEEE");
    exit(0);
    return 0;
}

int yd()
{
    printf("Yielding");
    while (true)
    {
        yield();
    }
    return 0;
}

int nmc()
{
    int code = wait_retcode(5);

    printf("Code: %d\n", code);

    exit(0);
    return 0;
}

int abc()
{
    exit(4);
    return 0;
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

pid_t create_process(char *name, uint32_t begin, int argc, char **argv)
{
    for (int z = 0; z < task_count; z++)
    {
        if (tasks[z]->null)
            continue;

        if (strcmp(tasks[z]->name, name) == 0)
            return -1;
    }

    task_t *task = (task_t *)malloc(sizeof(task_t *));

    memset(task->name, 0, 20);
    strcpy(task->name, name);

    task->pid = task_count;

    task->eip = (uint32_t)&wrapper;
    task->eflags = 0x202;
    task->null = false;
    task->blocked = false;

    task->start = begin;
    task->argc = argc;

    for (int z = 0; z < argc; z++)
        task->argv[z] = strdup(argv[z]);

    uint32_t stack_addr = allocate_stack();

    if (!stack_addr)
    {
        free(task);
        return -1;
    }

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

    task->esp = (uint32_t)stack;
    task->stack = stack_addr;

    tasks[task_count] = task;
    task_count++;

    return task->pid;
}

pid_t create_process_file(FILE *file, int argc, char **argv)
{
    if (argc <= 0)
        return -1;

    if (file != NULL)
    {
        auto header = load_elf_memory((uint8_t *)file->node->contents);
        return create_process(argv[0], header->e_entry, argc, argv);
    }
    else
    {
        return -1;
    }
}

pid_t create_process_filename(char *path, int argc, char **argv)
{
    if (argc <= 0)
        return -1;

    FILE *file = fopen(path, "r");

    if (file != NULL)
    {
        auto header = load_elf_memory((uint8_t *)file->node->contents);
        return create_process(argv[0], header->e_entry, argc, argv);
    }
    else
    {
        return -1;
    }
}

pid_t create_process(char *name, uint32_t begin)
{
    return create_process(name, begin, 0, NULL);
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

void wrapper()
{
    task_t *task = tasks[current_task];

    if (task == NULL)
    {
        asm("sti");
        exit(1);
    }

    call_t call = (call_t)task->start;

#ifdef DEBUG
    log::info("Loading task '%s' (PID %d) at address 0x%x\n", task->name, task->pid, task->start);
#endif

    int ret = call(task->argc, task->argv);
    exit(ret);
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

    char *argv[] = {"echo", "hello from echo"};
    create_process_filename("/bin/echo.o", 2, argv);

    tasking_on = true;
    switch_task(NULL, false);
}
