#pragma once

typedef struct
{
    fs_node_t *node;
} DIR;

int make_dir(char *name, char *dir, int permission);
int make_dir_user(char *name, char *dir);

int make_dir(char *name, char *dir);
int kmake_dir(char *name, char *dir);
