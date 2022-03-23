#pragma once

typedef struct
{
    int argc;
    char argv[64][64];
} args_t;

#ifdef __cplusplus
extern "C" {
#endif

extern bool there_is_more(char *command, int z);
extern args_t parse_args(char *command);

#ifdef __cplusplus
}
#endif
