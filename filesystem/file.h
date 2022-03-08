#pragma once

#include <filesystem/node.h>

#define EOF -1
#define SEEK_SET 0

typedef struct
{
    fs_node_t *node;

    int ptr;
    int eof;
} FILE;

typedef int fpos_t;

int create_file(char *name, char *path, char *contents, int permission);
int create_file(char *name, char *path, char *contents);
int kcreate_file(char *name, char *path, char *contents);

int create_file(char *name, char *path, __read read, __write write, int permission);
int create_file(char *name, char *path, __read read, __write write);
int kcreate_file(char *name, char *path, __read read, __write write);

char *fread(FILE *file, int offset, int size, char *buffer);

int fwrite(FILE *file, int offset, int size, char *buffer);

FILE *fopen(char *path);
void fclose(FILE *file);

int fgetc(FILE *file);
char *fgets(char *str, int n, FILE *file);

int feof(FILE *file);

int fgetpos(FILE *file, fpos_t *pos);
int fseek(FILE *file, int offset, int w);
int ftell(FILE *file);
void rewind(FILE *file);
int fsetpos(FILE *file, fpos_t *pos);

int fvsscanf(FILE *file, char *fmt, va_list va);
int fscanf(FILE *file, char *fmt, ...);
