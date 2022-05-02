#pragma once

#include "_header.h"

typedef struct {
    char *name;
    char *version;
    char *vendor;
    char cpu_name[30];
    bool dev;
    bool hypervisor;
    int model;
} info_t;

C_HEADER

extern info_t get_sys_info();

C_HEADER_END

#ifdef __cplusplus
namespace System {

extern info_t info();

}
#endif
