#include <syscalls.h>

extern "C" int mkdir(char *name, char *parent)
{
    int result[1];
    void * a;

    asm volatile("int $48" : "=a" (a) : "0" (MKDIR), "b" (name), "c" (parent), "d" (result));

    return result[0];
}
