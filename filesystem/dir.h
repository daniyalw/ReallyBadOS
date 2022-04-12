#pragma once

#include <filesystem/node.h>

typedef struct
{
    fs_node_t *node;
    int ptr = 0;
} DIR;

int make_dir(char *name, char *dir, int permission);
int make_dir_user(char *name, char *dir);

int make_dir(char *name, char *dir);
int kmake_dir(char *name, char *dir);

fs_node_t *readdir(DIR *dir);
DIR *opendir(char *name);
