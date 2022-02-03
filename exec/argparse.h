#pragma once

typedef struct
{
    int argc;
    char argv[64][64];
} args_t;

bool there_is_more(char *command, int z);
args_t parse_args(char *command);
