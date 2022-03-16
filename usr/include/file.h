#pragma once

#include <stdint.h>
#include <string.h>

#define EOF -1
#define SEEK_SET 0

struct fs_node;

#define FILENAME_LIMIT 20
#define CHILDREN_LIMIT 20

#define FS_FOLDER 0x1
#define FS_FILE 0x2

struct fs_node_t;

typedef int (*__write)(fs_node_t*, int, int, char *);
typedef int (*__read)(fs_node_t*, int, int, char *);

struct fs_node_t
{
    char name[FILENAME_LIMIT];
    char path[FILENAME_LIMIT * 10];

    int id, parent_id;
    int flags;

    int permission;

    bool null = false;

    int size = 0;
    char *contents;

    __write write = NULL;
    __read read = NULL;

    int children[CHILDREN_LIMIT];
    int children_count = NULL;
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

extern int fwrite(char *buf, int size, int n, FILE *file);
extern int fread(char *buffer, int size, int n, FILE *file);

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

struct FILE
{
    fs_node_t *node;

    int ptr;
    int eof;
};
