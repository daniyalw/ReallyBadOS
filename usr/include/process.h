#include <syscalls.h>
#include <stdint.h>
#include "_header.h"

typedef int pid_t;

C_HEADER

extern pid_t create_process(char *name, uint32_t instruction);
extern void yield();
extern void exit(int ret);

C_HEADER_END
