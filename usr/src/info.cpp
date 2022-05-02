#include <syscalls.h>
#include <mem.h>

typedef struct {
    char *name;
    char *version;
    char *vendor;
    char cpu_name[30];
    bool dev;
    bool hypervisor;
    int model;
} info_t;

extern "C" info_t get_sys_info()
{
    info_t info;
    void * a;
    CALL_SYS1(a, INFO, &info);

    return info;
}

namespace System {
    info_t info() {
        return get_sys_info();
    }
}
