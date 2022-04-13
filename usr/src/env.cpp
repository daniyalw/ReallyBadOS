#include <syscalls.h>
#include <stdlib.h>

EnvMap env_vec[100];
int env_count = 0;

int find_env(char *name) {
    for (int z = 0; z < env_count; z++) {
        if (strcmp(env_vec[z].name, name) == 0) {
            return z;
        }
    }

    return -1;
}

extern "C" char *getenv(char *name) {
    int pos = find_env(name);

    if (pos < 0) {
        return NULL;
    } else {
        return env_vec[pos].val;
    }
}

extern "C" int setenv(char *name, char *val, int overwrite) {
    int pos = -1;

    if ((pos = find_env(name)) != -1) {
        auto map = env_vec[pos];
        map.val = val;
        env_vec[pos] = map;
    } else {
        return 1;
    }

    if (overwrite) {
        int ret;
        CALL_SYS2(ret, SETENV, name, val);
        return ret;
    }

    return 0;
}

extern "C" int putenv(char *name) {
    if (getenv(name) != NULL) {
        return 1;
    }

    EnvMap map;
    map.name = strdup(name);
    map.val = NULL;
    env_vec[env_count] = map;
    env_count++;

    return 0;
}

extern "C" int get_env_count() {
    return env_count;
}
