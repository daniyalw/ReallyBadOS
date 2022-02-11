#include <stdio.h>
#include <stdarg.h>
#include <syscalls.h>
#include <stdint.h>

typedef void (*__write)(char *);
typedef char * (*__read)(char *);

struct fs_node
{
    char name[20];
    char path[100];
    int id;
    int parent_id;
    int flags;
    bool null = false;
    int size;

    int children_id[10]; // if the node is a folder
    int children_count = 0;

    char *contents; // if it is a file

    __write write;
    __read read;
};

typedef struct
{
    fs_node node;

    __write write;
    __read read;

    char name[20];

    bool null;
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

extern "C" int mkfile(char *name, char *dir, char *contents)
{
    void * a;
    int res[1];
    asm volatile("int $48" : "=a" (a) : "0" (MKDIR), "b" (name), "c" (dir), "d" (contents), "S" (res));

    return res[0];
}
