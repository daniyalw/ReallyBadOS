#include <stdio.h>
#include <stdarg.h>
#include <syscalls.h>
#include <stdint.h>

typedef void (*__write)(char *);
typedef char * (*__read)(char *);

typedef struct
{
    char * name;
    char * contents;
    char * path;
    char * parent;
    int id;
    uint32_t size;
    bool null = false;

    bool operator!() { return null; }

    __write write;
    __read read;
} FILE;

extern "C" FILE *fopen(char *path)
{
    void *a;
    uint32_t *addr;

    asm volatile("int $48" : "=a" (a) : "0" (FOPEN), "b" (path), "c" (addr));

    FILE *file = (FILE *)(addr[0]);

    return (FILE *)addr[0];
}

extern "C" void fclose(FILE *file)
{
    void *a;

    asm volatile("int $48" : "=a" (a) : "0" (FCLOSE), "b" (file));
}

extern "C" int fexec(char *contents)
{
    void *a;
    int *ret;

    asm volatile("int $48" : "=a" (a) : "0" (FEXEC), "b" (contents), "c" (ret));

    return ret[0];
}

extern "C" void ls(char *path)
{
    void *a;
    asm volatile("int $48" : "=a" (a) : "0" (FLS), "b" (path));
}
