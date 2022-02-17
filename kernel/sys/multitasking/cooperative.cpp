#include <sys/multitasking/cooperative.h>

using namespace Cooperative;

namespace Cooperative {

void create_task(char * name, void (*func)())
{
    ASSERT(task_count < 10);
    task_t task;

    task.name = name;
    task.func = func;

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

    current.func();
}

void yield()
{
    switch_task();
}

void update_addr(void (*func)())
{
    current.func = func;
    tasks[current_id] = current;
}

void task_ls()
{
    // task_count is not true reflection of available tasks, since it includes NULL tasks
    int count = 0;

    for (int z = 0; z < task_count; z++)
    {
        if (!tasks[z].null) count++;
    }

    printf("Tasks: %d\n", count);

    for (int z = 0; z < task_count; z++)
    {
        if (tasks[z].null) continue;

        printf("Task: %s", tasks[z].name);
        putchar('\n');
    }
}

bool task_exists(char *proc)
{
    for (int z = 0; z < task_count; z++)
    {
        if (strcmp(tasks[z].name, proc) == 0)
            return true;
    }

    return false;
}

int task_id(char *proc)
{
    if (!task_exists(proc)) return -1;

    for (int z = 0; z < task_count; z++)
    {
        if (strcmp(tasks[z].name, proc) == 0)
            return z;
    }

    return -1;
}

void kill_proc(char *proc)
{
    int z = task_id(proc);

    if (z != -1)
    {
        task_t task = tasks[z];
        task.null = true;
        tasks[z] = task;
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
    yield();
}

void second_idle()
{
    yield();
}

void init_cooperative_tasking()
{
    create_task("idle1", idle_task);
    create_task("idle2", second_idle);

    switch_task();
}
