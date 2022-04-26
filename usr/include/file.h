#pragma once

#include <stdint.h>
#include <string.h>
#include "_header.h"

#define EOF -1
#define SEEK_SET 0
#define SEEK_END -1
#define SEEK_CUR -2
#define PATH_LIMIT 256

struct fs_node;

#define FILENAME_LIMIT 20
#define CHILDREN_LIMIT 20

#define FS_FOLDER 0x1
#define FS_FILE 0x2

struct fs_node_t;

typedef int (*__open)(fs_node_t*);
typedef int (*__close)(fs_node_t*);
typedef int (*__write)(fs_node_t*, int, int, char *);
typedef int (*__read)(fs_node_t*, int, int, char *);
typedef int (*__mkfile)(fs_node_t*);
typedef int (*__mkdir)(fs_node_t *);

struct fs_driver_t {
    __open open;
    __close close;
    __write write;
    __read read;
    __mkfile mkfile;
    __mkdir mkdir;
};

struct fs_node_t
{
    char name[FILENAME_LIMIT];
    char path[PATH_LIMIT];

    int id, parent_id;
    int flags;

    int permission, owner;

    bool null = false;

    char *contents;

    bool is_mountpoint;
    char mount_dir[FILENAME_LIMIT];
    int mount_parent;
    bool is_mount;

    __mkfile mkfile = NULL;
    __write write = NULL;
    __read read = NULL;
    __mkdir mkdir = NULL;
    __open open;
    __close close;
    __get_size get_size;

    int children[CHILDREN_LIMIT];
    int children_count = NULL;
};

struct FILE;
typedef int fpos_t;

C_HEADER

extern FILE *fopen(char *path, char *mode);
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
extern int fseek(FILE *file, int offset, int whence);
extern int fsize(FILE *file);
extern int ftell(FILE *file);
extern void rewind(FILE *file);
extern int fsetpos(FILE *file, fpos_t *pos);
extern int fprintf(FILE *stream, char *format, ...);

extern int fileno(FILE *stream);

C_HEADER_END

struct FILE
{
    fs_node_t *node;

    int ptr;
    int eof;
    int flags;
};
