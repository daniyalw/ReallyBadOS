#include <syscalls.h>

extern "C" int mkdir(char *name, char *parent)
{
    int ret;

    CALL_SYS2(ret, MKDIR, name, parent);

    return ret;
}
