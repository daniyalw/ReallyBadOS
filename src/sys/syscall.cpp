static void syscall_handler(registers_t regs)
{
    int r;
    asm volatile("\
                  push %1; \
                  push %2; \
                  push %3; \
                  push %4; \
                  push %5; \
                  call *%6; \
                  pop %%ebx; \
                  pop %%ebx; \
                  pop %%ebx; \
                  pop %%ebx; \
                  pop %%ebx; \
                  ": "=a" (r) : "r" (regs.edi), "r" (regs.esi), "r" (regs.edx), "r" (regs.ecx), "r" (regs.ebx), "r" (&print));
    regs.eax = r;
    return;
}

void init_syscall()
{
    register_interrupt_handler(IRQ3, syscall_handler);
}
