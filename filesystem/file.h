#pragma once

#include <filesystem/node.h>

struct FILE;

#define EOF -1
#define SEEK_SET 0

int fwrite(char *buf, int offset, int size, FILE *file);
char * fread(char *buf, int offset, int size, FILE *file);

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

typedef int fpos_t;

FILE *fopen(char *path);
void fclose(FILE *file);

int create_file(char *path, char *folder, char *contents);
int create_file(char *path, char *folder, __read read, __write write);

void fprintf(FILE *file, char *data);
void fprintf(FILE file, char *data);
void fprintf(fs_node node, char *data);

char *fread(char *buf, FILE *file);
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
