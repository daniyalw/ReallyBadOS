#pragma once

#include <stdint.h>
#include <string.h>

struct fs_node;

typedef void (*__write)(fs_node, int offset, int size, char *);
typedef char * (*__read)(fs_node, int offset, int size, char *);

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

struct FILE;

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *fopen(char *path);
extern void fclose(FILE *file);
extern int fexec(char *contents);
extern void ls(char *path);
extern int mkfile(char *name, char *dir, char *contents);
extern int write_file(char *file, char *contents);
extern void append_file(char *name, char *contents);
extern char *read_file(char *file, char *out);

#ifdef __cplusplus
}
#endif

typedef struct FILE
{
    fs_node node;

    int write(char *buf) { return write_file(node.path, buf); }
    char *read(char *buf) { buf = read_file(node.path, buf); return buf; }

    char name[20];

    bool null;
} FILE;
