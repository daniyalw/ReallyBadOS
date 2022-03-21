#include <syscalls.h>
#include <mem.h>

typedef struct
{
    char *name;
    char *version;
    bool dev;
} info_t;

extern "C" info_t get_sys_info()
{
    info_t info[1];
    void * a;
    CALL_SYS1(a, INFO, info);

    return info[0];
}
