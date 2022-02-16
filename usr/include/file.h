#pragma once

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

    int children_id[12]; // if the node is a folder
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

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *fopen(char *path);
extern void fclose(FILE *file);
extern int fexec(char *contents);
extern void ls(char *path);
extern int mkfile(char *name, char *dir, char *contents);
extern int write_file(char *name, char *contents);
extern void append_file(char *name, char *contents);

#ifdef __cplusplus
}
#endif
