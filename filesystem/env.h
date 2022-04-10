#pragma once

#include "node.h"

struct EnvMap {
    char *name;
    char *val;
};

EnvMap env_vec[100];
int env_count = 0;
fs_node_t *env_node;

char *getenv(char *name);
int setenv(char *name, char *val, int overwrite);
int env_init();
