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
    if (began)
        tasks[current_id] = current;

    int id = current_id;

    id++;

    if (id >= task_count)
        id = 0;

    current_id = id;

    current = tasks[id];

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
}

}
