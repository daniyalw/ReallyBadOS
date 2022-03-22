#pragma once

extern "C"
{
    extern void perform_task_switch(uint32_t, uint32_t, uint32_t);
    extern uint32_t read_eip();
}

typedef struct
{
    char name[20];
    int eip, esp, ebp, eflags;
    int pid;
    uint32_t stack;
    bool null, blocked;
    int ret;

    uint32_t start;
    int argc;
    char *argv[64];
} task_t;

task_t *tasks[30];
int current_task = 0;
int task_count = 0;

int tasking_on = false;

typedef int pid_t;

pid_t create_process(char *name, uint32_t begin, int argc, char **argv);
pid_t create_process(char *name, uint32_t begin);
pid_t create_process_file(FILE *file, int argc, char **argv);
pid_t create_process_filename(char *path, int argc, char **argv);

void load_new_task(task_t *task);

void save_task_context(task_t *task, registers_t *regs);

uint32_t allocate_stack();

void init_tasking();
void switch_task(registers_t *regs, bool save);

void exit(int ret);
void yield();

void block_task(task_t *task);
void block_task_id(int taskid);
void unblock_task(task_t *task);
void unblock_task_id(int taskid);

int wait_retcode(pid_t pid);
void wrapper();
