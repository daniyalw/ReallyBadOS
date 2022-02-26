#pragma once

typedef struct
{
    char *name;
    char *version;
    bool dev;
} info_t;

#ifdef __cplusplus
extern "C" {
#endif

extern info_t get_sys_info();

#ifdef __cplusplus
}
#endif
