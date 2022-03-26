#include <sys/descriptors/isr.h>

#define PRINT 0
#define PUTCHAR 1
#define RANDOM 2
#define TIME 3
#define PUTPIXEL 4
#define UPDATE_MOUSE 5
#define INFO 6

#define DEFN_SYSCALL0(fn, num) \
void syscall_##fn() { \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num)); \
}

#define DEFN_SYSCALL1(fn, num, P1) \
void syscall_##fn(P1 p1) { \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1)); \
}

#define DEFN_SYSCALL2(fn, num, P1, P2) \
void syscall_##fn(P1 p1, P2 p2) { \
    void * a; \
 asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2)); \
}

#define DEFN_SYSCALL3(fn, num, P1, P2, P3) \
void syscall_##fn(P1 p1, P2 p2, P3 p3) { \
    void * a; \
    asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2), "d" (p3)); \
}

#define DEFN_SYSCALL4(fn, num, P1, P2, P3, P4) \
void syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4) { \
    void * a; \
    asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2), "d" (p3), "S" (p4)); \
}

#define DEFN_SYSCALL5(fn, num, P1, P2, P3, P4, P5) \
void syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) { \
    void * a; \
    asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2), "d" (p3), "S" (p4), "D" (p5)); \
}

void init_syscalls();
static void syscall_handler(registers_t regs);
void syscall_append(void *func);

int max_syscalls = 30;
void *syscalls[30];
int syscall_count = 0;
