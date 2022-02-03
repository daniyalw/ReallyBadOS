#pragma once

namespace Cooperative {

typedef struct
{
    char *name;
    void (*func)();
    bool null = false;
} task_t;

task_t tasks[10];
int task_count = 0;

task_t current;
int current_id = 0;

bool began = false;

void create_task(char * name, void (*func)());
void switch_task();
void yield();
void update_addr(void (*func)());
void exit();
void task_ls();

bool task_exists(char *proc);
int task_id(char *proc);
void kill_proc(char *proc);
}
