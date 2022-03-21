#include <syscalls.h>
#include <stdint.h>

extern "C" uint32_t malloc(int size)
{
    uint32_t buf[1];
    int sz[1];
    void *a;

    sz[0] = size;

    CALL_SYS2(a, MALLOC, sz, buf);

    return buf[0];
}

extern "C" void free(void *buf)
{
    void *a;
    CALL_SYS1(a, FREE, buf);
}
