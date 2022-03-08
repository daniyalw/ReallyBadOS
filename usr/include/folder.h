#pragma once

typedef struct
{
    fs_node_t *node;
} DIR;

#ifdef __cplusplus
extern "C" {
#endif

extern int mkdir(char *name, char *parent);

#ifdef __cplusplus
}
#endif
