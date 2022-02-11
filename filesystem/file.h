#pragma once

#include <filesystem/node.h>

typedef struct
{
    fs_node node;

    __write write;
    __read read;

    char name[20];

    bool null;
} FILE;

FILE *fopen(char *path);
void fclose(FILE *file);
int create_file(char *path, char *folder, char *contents);
int create_file(char *path, char *folder, __read read, __write write);
