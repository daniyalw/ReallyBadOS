#include <kernel/isr.h>

#define DEFN_SYSCALL0(fn, num) \
void syscall_##fn() \
{ \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num)); \
}

#define DEFN_SYSCALL1(fn, num, P1) \
void syscall_##fn(P1 p1) \
{ \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1)); \
}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
void syscall_##fn(P1 p1, P2 p2) \
{ \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2)); \
}

void init_syscalls();
static void syscall_handler(registers_t regs);
void syscall_append(void *func);

std::list<void *> syscalls;
