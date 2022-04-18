#include "task.h"
#include "scheduler.h"
#include "thread.h"
#include "ipc.h"
#include <shell/shell.h>

namespace Kernel {

namespace CPU {

int idle_task() {
    while (true) {}
    return 0;
}

int ipc_test() {
    auto msg = ipc_send_wait(2, strdup("Hello!"));

    if (msg) printf("Msg: %s\n", msg->data);
    else printf("Msg is NULL\n");

    return 0;
}

int ipc_test11() {
    while (true) {
        Message *msg = ipc_read_last_msg();

        if (msg != NULL) {
            printf("Received message: %s\n", msg->data);
            ipc_send_msg(msg->from, strdup("Bye!"));
            free(msg->data);
            ipc_msg_finish(msg);
            exit(0);
        }
    }

    return 0;
}

int task_err_check() {
    jmp_somewhere(0); // jump to null
    return 0;
}

void block_task(task_t *task) {
    task->blocked = true;
    tasks[task->pid] = task;
}

void block_task_id(int taskid) {
    ASSERT(taskid >= 0 && taskid < task_count);
    task_t *task = tasks[taskid];
    block_task(task);
}

void unblock_task(task_t *task) {
    task->blocked = false;
    tasks[task->pid] = task;
}

void unblock_task_id(int taskid) {
    ASSERT(taskid >= 0 && taskid < task_count);
    task_t *task = tasks[taskid];
    unblock_task(task);
}

int find_retcode(pid_t pid) {
    ASSERT(pid >= 0 && pid < task_count);

    if (tasks[pid]->null)
        return tasks[pid]->ret;
    else
        return -1;
}

int wait_retcode(pid_t pid) {
    ASSERT(pid >= 0 && pid < task_count);

    while (true) {
        if (tasks[pid]->null)
            return tasks[pid]->ret;
        else
            yield();
    }
}

void exit(int ret) {
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

void yield() {
    task_counter = 0;
    switch_task(NULL, true);
}

void abort() {
    exit(1);
}

pid_t create_process(char *name, uint32_t begin, bool thread, int parent, int argc, char **argv, int user) {
    // threads aren't allowed to create processes
    if (parent >= 0 && tasks[parent]->is_thread)
        return -1;

    // make sure no other process/thread has the same name
    for (int z = 0; z < task_count; z++) {
        if (tasks[z]->null)
            continue;

        if (strcmp(tasks[z]->name, name) == 0)
            return -1;
    }

    task_t *task = (task_t *)malloc(sizeof(task_t));

    memset(task->name, 0, 20);
    strcpy(task->name, name);

    task->pid = task_count;

    task->eip = (uint32_t)&wrapper;
    if (user) {
        task->eflags = 0x206;
    } else {
        task->eflags = 0x202;
    }
    task->null = false;
    task->blocked = false;
    task->is_thread = thread;
    task->parent = parent;

    task->start = begin;
    task->argc = argc;

    for (int z = 0; z < argc; z++)
        task->argv[z] = strdup(argv[z]);

    uint32_t stack_addr = allocate_stack(); // allocate 4K for the stack

    if (!stack_addr) {
        free(task);
        return -1;
    }

    uint32_t *stack = (uint32_t *)stack_addr + (4 * 1024);

    // init stack
    *--stack = task->eflags; // eflags
    if (user) {
        *--stack = 0x1b; // cs
    } else {
        *--stack = 0x0b;
    }
    *--stack = (uint32_t)begin; // eip
    *--stack = 0; // eax
    *--stack = 0; // ebx
    *--stack = 0; // ecx;
    *--stack = 0; // edx
    *--stack = 0; // esi
    *--stack = 0; // edi
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

void kill(int pid, int ret) {
    if (pid < 0 || pid > task_count) {
        return;
    }

    task_t *task = tasks[pid];

    // let's not be able to kill random processes at will
    if (task->parent != current_task) {
        return;
    }

    task->ret = ret;
    free((void *)task->stack);
    task->null = true;

    tasks[pid] = task;
}

void kill(int pid) {
    kill(pid, 0);
}

pid_t create_process_file(FILE *file, int argc, char **argv) {
    if (argc <= 0)
        return -1;

    if (file != NULL) {
        auto header = load_elf_memory((uint8_t *)file->node->contents);
        return create_process(argv[0], header->e_entry, false, -1, argc, argv, 1);
    } else {
        return -1;
    }
}

pid_t create_process_filename(char *path, int argc, char **argv) {
    if (argc <= 0)
        return -1;

    FILE *file = fopen(path, "r");
    int ret = create_process_file(file, argc, argv);

    return ret;
}

pid_t create_process(char *name, uint32_t begin) {
    return create_process(name, begin, false, -1, 0, NULL, 0);
}

void load_new_task(task_t *task) {
    uint32_t esp, eip, ebp;
    esp = task->esp;
    eip = task->eip;
    ebp = task->ebp;

    perform_task_switch(eip, ebp, esp);
}

uint32_t allocate_stack() {
    return malloc(4 * 1024);
}

// this is useful since passing argv via stack doesn't work
void wrapper() {
    task_t *task = tasks[current_task];

    if (task == NULL || task->null || task->blocked) {
        exit(1);
    }

    call_t call = (call_t)task->start;

    int ret = call(task->argc, task->argv, env_vec, env_count);

#ifdef DEBUG
    log::info("Application '%s' exited with a return value %d", task->name, ret);
#endif

    exit(ret);
}

void switch_task(registers_t *regs, bool save) {
    uint32_t esp, ebp;
    asm volatile ("mov %%esp, %0" : "=r" (esp)); // save the stack
	asm volatile ("mov %%ebp, %0" : "=r" (ebp)); // save the ebp
    uint32_t eip = read_eip(); // save the current instruction pointer

    if (eip == 0x12344)
        return;

    task_t *current = tasks[current_task];

    if (save) {
        current->eip = eip;
        current->esp = esp;
        current->ebp = ebp;

        tasks[current->pid] = current;
    }

    Kernel::CPU::schedule();

    task_t *load = tasks[current_task];
#ifdef DEBUG
    log::warning("New task load: %s (type %s, parent %d, pid %d)\nNew task eip: 0x%x\nNew task start: 0x%x",
    load->name,
    (char *)(load->is_thread ? "thread" : "process"),
    load->parent,
    load->pid,
    load->eip,
    load->start);
#endif
    load_new_task(load);
}

int adddd() {
    printf("Exiting..");
    return 5;
}

void init_tasking() {
    create_process("idle", (uint32_t)&idle_task);
    create_process("ipc-test", (uint32_t)&ipc_test);
    create_process("ipc-test-1", (uint32_t)&ipc_test11);
    //create_process("err-check", (uint32_t)&task_err_check);
    //create_process("shell", (uint32_t)&shell);
    char *argv[] = {"echo", "hello"};
    create_process_filename("/bin/echo.o", 2, argv);

    tasking_on = true;
    switch_task(NULL, false);
}

}

}
