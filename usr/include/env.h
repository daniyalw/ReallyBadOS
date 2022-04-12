#pragma once

#include "_header.h"

C_HEADER

extern char *getenv(char *name);
extern int setenv(char *name, char *val, int overwrite);

C_HEADER_END
