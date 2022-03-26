#pragma once
#include <sys/io.h>
#include <string.h>
#include <sys/descriptors/interrupts.h>

namespace Kernel {

static inline bool are_interrupts_enabled() {
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

static inline unsigned long save_irqdisable() {
    unsigned long flags;
    asm volatile ("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
    return flags;
}

static inline void irqrestore(unsigned long flags) {
    asm ("push %0\n\tpopf" : : "rm"(flags) : "memory","cc");
}

static inline void disable_interrupts() {
    asm volatile("cli");
}

static inline void enable_interrupts() {
    asm volatile("sti");
}

static inline void call_interrupt(int int_no) {
    switch (int_no) {
        case 1:
            asm volatile("int $1");
            break;
        case 2:
            asm volatile("int $2");
            break;
        case 3:
            asm volatile("int $3");
            break;
        case 4:
            asm volatile("int $4");
            break;
        case 5:
            asm volatile("int $5");
            break;
        case 6:
            asm volatile("int $6");
            break;
        case 7:
            asm volatile("int $7");
            break;
        case 8:
            asm volatile("int $8");
            break;
        case 9:
            asm volatile("int $9");
            break;
        case 10:
            asm volatile("int $10");
            break;
        case 11:
            asm volatile("int $11");
            break;
        case 12:
            asm volatile("int $12");
            break;
        case 13:
            asm volatile("int $13");
            break;
        case 14:
            asm volatile("int $14");
            break;
        case 15:
            asm volatile("int $15");
            break;
        case 16:
            asm volatile("int $16");
            break;
        case 17:
            asm volatile("int $17");
            break;
        case 18:
            asm volatile("int $18");
            break;
        case 19:
            asm volatile("int $19");
            break;
        case 20:
            asm volatile("int $20");
            break;
        case 21:
            asm volatile("int $21");
            break;
        case 22:
            asm volatile("int $22");
            break;
        case 23:
            asm volatile("int $23");
            break;
        case 24:
            asm volatile("int $24");
            break;
        case 25:
            asm volatile("int $25");
            break;
        case 26:
            asm volatile("int $26");
            break;
        case 27:
            asm volatile("int $27");
            break;
        case 28:
            asm volatile("int $28");
            break;
        case 29:
            asm volatile("int $29");
            break;
        case 30:
            asm volatile("int $30");
            break;
        case 31:
            asm volatile("int $31");
            break;
    }
}

}
