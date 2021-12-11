#include <kernel/isr.h>

#define DECL_SYSCALL0(fn) void sys_##fn();
#define DECL_SYSCALL1(fn,p1) void sys_##fn(p1);
#define DECL_SYSCALL2(fn,p1,p2) void sys_##fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3) void sys_##fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4) void sys_##fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5) void sys_##fn(p1,p2,p3,p4,p5);
#define DECL_SYSCALL6(fn,p1,p2,p3,p4,p5,p6) void sys_##fn(p1,p2,p3,p4,p5,p6);

#define DEFN_SYSCALL0(fn, num) \
void sys_##fn() \
{ \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num)); \
}

#define DEFN_SYSCALL1(fn, num, P1) \
void sys_##fn(P1 p1) \
{ \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1)); \
}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
void sys_##fn(P1 p1, P2 p2) \
{ \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2)); \
}

void init_syscalls();
static void syscall_handler(registers_t regs);
void syscall_append(void *func);

std::list<void *> syscalls;
