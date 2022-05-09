#include <syscalls.h>
#include <stdint.h>
#include "_header.h"

typedef int pid_t;

C_HEADER

extern pid_t create_process(char *name, uint32_t instruction);
extern void yield();
extern void exit(int ret);
extern char *name_from_pid(int pid);
extern int pid_from_name(char *name);

C_HEADER_END

#ifdef __cplusplus
namespace Process {

extern pid_t create(char *name, uint32_t instruction);
extern void next();
extern void leave(int ret);

}
#endif
