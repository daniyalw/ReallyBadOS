#pragma once

#include "_header.h"

typedef struct
{
    int argc;
    char argv[64][64];
} args_t;

C_HEADER

extern bool there_is_more(char *command, int z);
extern args_t parse_args(char *command);

C_HEADER_END
