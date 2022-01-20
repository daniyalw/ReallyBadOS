#include <kernel/multitasking/cooperative.h>

using namespace Cooperative;

namespace Cooperative {

void create_task(char * name, uint32_t addr)
{
    ASSERT(task_count < 10);
    task_t task;

    task.name = name;
    task.addr = addr;

    tasks[task_count] = task;
    task_count++;
}

void switch_task()
{

    int id = current_id;

    id++;

    if (id >= task_count)
        id = 0;

    current_id = id;
    bool found = false;

    current = tasks[id];

    if (tasks[id].null)
    {
        bool found = false;

        for (int z = 0; z < task_count; z++)
        {
            if (tasks[z].null)
            {
            }
            else
            {
                found = true;
                break;
            }
        }

        if (found)
            switch_task();
        else
        {
            task_ls();
            PANIC("No available task!\n");
        }
    }

    began = true;

    jmp_somewhere(current.addr);
}

void yield()
{
    switch_task();
}

void update_addr(uint32_t addr)
{
    current.addr = addr;
    tasks[current_id] = current;
}

void task_ls()
{
    for (int z = 0; z < task_count; z++)
    {
        printf("\nTask%s: %s", (char *)(tasks[z].null ? " (NULL)" : ""), tasks[z].name);
    }
}

void exit()
{
    current.null = true;
    tasks[current_id] = current;
    yield();
}

}

void idle_task()
{
    FILE *file = fopen("/usr/hello.txt");
    file->write("Hello!");
    yield();
}

void second_idle()
{
    FILE *file = fopen("/usr/hello.txt");
    file->write("Hello!");
    yield();
}

void init_cooperative_tasking()
{
    create_task("idle1", (uint32_t)&idle_task);
    create_task("idle2", (uint32_t)&second_idle);

    switch_task();
}
