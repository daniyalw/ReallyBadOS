#pragma once

#include <stdint.h>
#include <string.h>

#define EOF -1
#define SEEK_SET 0

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
typedef int fpos_t;

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *fopen(char *path);
extern void fclose(FILE *file);
extern int fexec(char *contents);
extern void ls(char *path);
extern int mkfile(char *name, char *dir, char *contents);

extern int fwrite(char *buf, int offset, int size, FILE *file);
extern char * fread(char *buf, int offset, int size, FILE *file);

extern int fgetc(FILE *file);
extern int feof(FILE *file);
extern char *fgets(char *str, int n, FILE *file);
extern int fgetpos(FILE *file, fpos_t *pos);
extern int fseek(FILE *file, int offset, int w);
extern int ftell(FILE *file);
extern void rewind(FILE *file);
extern int fsetpos(FILE *file, fpos_t *pos);

#ifdef __cplusplus
}
#endif

typedef struct FILE
{
    fs_node node;

    int write(char *buf, int offset, int size) { return fwrite(buf, offset, size, this); }
    char *read(char *buf, int offset, int size) { buf = fread(buf, offset, size, this); return buf; }

    char name[20];

    int ptr = 0;
    int eof = 0;

    bool null;
} FILE;
