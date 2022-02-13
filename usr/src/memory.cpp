#include <syscalls.h>
#include <stdint.h>

extern "C" uint32_t malloc(int size)
{
    uint32_t buf[1];
    int sz[1];
    void *a;

    sz[0] = size;

    asm volatile("int $48" : "=a" (a) : "0" (MALLOC), "b" (sz), "c" (buf));

    return buf[0];
}

extern "C" void free(void *buf)
{
    void *a;
    asm volatile("int $48" : "=a" (a) : "0" (FREE), "b" (buf));
}
