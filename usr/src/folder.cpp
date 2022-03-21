#include <syscalls.h>

extern "C" int mkdir(char *name, char *parent)
{
    int ret;

    asm volatile("int $48" : "=a" (ret) : "0" (MKDIR), "b" (name), "c" (parent));

    return ret;
}
