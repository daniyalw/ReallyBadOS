#include <process.h>

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

namespace Process {
    pid_t proc(char *name, uint32_t instruction) {
        return create_process(name, instruction);
    }

    void next() {
        yield();
    }

    void leave(int ret) {
        exit(ret);
    }

}
