#pragma once

#include <filesystem/node.h>

struct FILE;

int file_write(FILE *file, char *buf);
char * file_read(FILE *file, char *buf);

typedef struct FILE
{
    fs_node node;

    int write(char *buf) { return file_write(this, buf); }
    char *read(char *buf) { buf = file_read(this, buf); return buf; }

    char name[20];

    bool null;
} FILE;

FILE *fopen(char *path);
void fclose(FILE *file);

int create_file(char *path, char *folder, char *contents);
int create_file(char *path, char *folder, __read read, __write write);

void fprintf(FILE *file, char *data);
void fprintf(FILE file, char *data);
void fprintf(fs_node node, char *data);
