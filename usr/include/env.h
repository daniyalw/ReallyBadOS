#pragma once

#include "_header.h"

struct EnvMap {
    char *name;
    char *val;
};

C_HEADER

extern char *getenv(char *name);
extern int setenv(char *name, char *val, int overwrite);
extern int get_env_count();
extern int putenv(char *name);
int get_env_count();

C_HEADER_END
