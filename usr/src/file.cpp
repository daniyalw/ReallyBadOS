#include <stdio.h>
#include <stdarg.h>
#include <syscalls.h>
#include <stdint.h>
#include <file.h>

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

extern "C" int mkfile(char *name, char *dir, char *contents)
{
    void * a;
    int res[1];
    asm volatile("int $48" : "=a" (a) : "0" (MKDIR), "b" (name), "c" (dir), "d" (contents), "S" (res));

    return res[0];
}

extern "C" int write_file(FILE *file, char *contents)
{
    void *a;
    int res[1];
    asm volatile("int $48" : "=a" (a) : "0" (WRITE_FILE), "b" (file), "c" (contents), "d" (res));

    return res[0];
}

extern "C" void append_file(char *name, char *contents)
{
    void *a;
    asm volatile("int $48" : "=a" (a) : "0" (APPEND_FILE), "b" (name), "c" (contents));
}


extern "C" char *read_file(FILE *file, char *out)
{
    void *a;
    asm volatile("int $48" : "=a" (a) : "0" (READ_FILE), "b" (file), "c" (out));

    return out;
}
