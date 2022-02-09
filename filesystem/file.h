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
