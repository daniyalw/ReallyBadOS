#include <panic.h>
#include <stdio.h>
#include <sys/descriptors/interrupts.h>
#include <sys/cpu/halt.h>
#include <sys/log/log.h>
#include <stdlib/va_list.h>

void panic(char * msg, char * file, char * function, int line, ...)
{
    va_list va;

    va_start(va, line);
    msg = vsprintf(msg, msg, va);
    va_end(va);

    clear();

    log::error("\n\nPANIC:\n\tMessage: %s\n\n\tFile: %s\n\tFunction: %s\n\tLine: %d\n\n", msg, file, function, line);
    printf("\n\nPANIC:\n\tMessage: %s\n\n\tFile: %s\n\tFunction: %s\n\tLine: %d\n\n", msg, file, function, line);

    Kernel::disable_interrupts();
    Kernel::halt_system();
}

void traceback(int limit)
{
    uint32_t *ebp = (uint32_t *)__builtin_frame_address(0);
    int z = 0;

    for (;;) {
        if (z == limit)
            break;

        uint32_t old_ebp = ebp[0];
        uint32_t ret_address = ebp[1];

        if (!ret_address)
            break;

        printf("0x%x\n", ret_address);

        if (!old_ebp)
            break;

        ebp = (uint32_t *)old_ebp;
        z++;
    }
}
