#pragma once

#include <filesystem/node.h>

#define EOF -1
#define SEEK_SET 0
#define SEEK_END -1
#define SEEK_CUR -2

#define FLAGS_READ 4
#define FLAGS_WRITE 5
#define FLAGS_NONE 6
#define FLAGS_BYTES 7

typedef struct
{
    fs_node_t *node;

    int ptr;
    int eof;
    int flags;
} FILE;

typedef int fpos_t;

int create_file(char *name, char *path, int permission);
int create_file(char *name, char *path);

int create_file(char *name, char *path, char *contents, int permission);
int create_file(char *name, char *path, char *contents);
int kcreate_file(char *name, char *path, char *contents);

int create_file(char *name, char *path, __read read, __write write, int permission);
int create_file(char *name, char *path, __read read, __write write);
int kcreate_file(char *name, char *path, __read read, __write write);

int fread(FILE *file, int size, int n, char *buffer);

int fwrite(FILE *file, int size, int n, char *buffer);

FILE *fopen(char *path, char *mode);
void fclose(FILE *file);

int fgetc(FILE *file);
char *fgets(char *str, int n, FILE *file);

int feof(FILE *file);

int fgetpos(FILE *file, fpos_t *pos);
int fseek(FILE *file, int offset, int whence);
int ftell(FILE *file);
void rewind(FILE *file);
int fsetpos(FILE *file, fpos_t *pos);
int fsize(FILE *file);

int fvsscanf(FILE *file, char *fmt, va_list va);
int fscanf(FILE *file, char *fmt, ...);

int get_flags_from_str(char *mode);

int fprintf(FILE *file, char *data);
int fprintf(fs_node_t *file, char *data);
