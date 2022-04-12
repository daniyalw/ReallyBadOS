#pragma once

#include "_header.h"

typedef struct
{
    fs_node_t *node;
} DIR;

C_HEADER

extern int mkdir(char *name, char *parent);

C_HEADER_END
