#pragma once

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

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *fopen(char *path);
extern void fclose(FILE *file);
extern int fexec(char *contents);
extern void ls(char *path);

#ifdef __cplusplus
}
#endif
