#include <syscalls.h>
#include <stdlib.h>

extern "C" char *getenv(char *name) {
    char *env = (char *)malloc(100);
    memset(env, 0, 100);
    void *a;
    CALL_SYS2(a, GETENV, name, env);
    return env;
}

extern "C" int setenv(char *name, char *val, int overwrite) {
    if (overwrite) {
        int ret;
        CALL_SYS2(ret, SETENV, name, val);
        return ret;
    }

    return 1;
}
