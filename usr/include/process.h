#include <syscalls.h>
#include <stdint.h>

typedef int pid_t;

#ifdef __cplusplus
extern "C" {
#endif

extern pid_t create_process(char *name, uint32_t instruction);
extern void yield();
extern void exit(int ret);

#ifdef __cplusplus
}
#endif
