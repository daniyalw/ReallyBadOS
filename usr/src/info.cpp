#include <syscalls.h>

typedef struct
{
    char name[20];
    char version[10];
} info_t;

extern "C" info_t *get_sys_info()
{
    info_t *info;
    void * a;
    asm volatile("int $48" : "=a" (a) : "0" (INFO), "b" (info));

    return info;
}
