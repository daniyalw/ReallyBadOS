#pragma once

typedef struct
{
    char name[20];
    char version[10];
} info_t;

#ifdef __cplusplus
extern "C" {
#endif

extern info_t *get_sys_info();

#ifdef __cplusplus
}
#endif
