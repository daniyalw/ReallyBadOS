#include <process.h>
#include <mem.h>
#include <string.h>

extern "C" pid_t create_process(char *name, uint32_t instruction)
{
    pid_t pid = -1;
    CALL_SYS2(pid, CREATE_PROC, name, instruction);
    return pid;
}

extern "C" void yield()
{
    void *a;
    CALL_SYS0(a, YIELD_PROC);
}

extern "C" void exit(int ret)
{
    void *a;
    CALL_SYS1(a, EXIT_PROC, ret);
}

extern "C" char *name_from_pid(int pid) {
    void *a;
    char *buffer = (char *)malloc(100);
    memset(buffer, 0, 100);
    CALL_SYS2(a, TASKING_NAME, pid, buffer);
    return buffer;
}

extern "C" int pid_from_name(char *name) {
    int pid = -1;
    CALL_SYS1(pid, TASKING_PID, name);
    return pid;
}

namespace Process {
    pid_t create(char *name, uint32_t instruction) {
        return create_process(name, instruction);
    }

    void next() {
        yield();
    }

    void leave(int ret) {
        exit(ret);
    }
}
