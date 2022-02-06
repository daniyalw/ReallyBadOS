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

    Kernel::system_log("\n\nPANIC:\n\tMessage: %s\n\n\tFile: %s\n\tFunction: %s\n\tLine: %d\n\n", msg, file, function, line);
    printf("\n\nPANIC:\n\tMessage: %s\n\n\tFile: %s\n\tFunction: %s\n\tLine: %d\n\n", msg, file, function, line);

    Kernel::disable_interrupts();
    Kernel::halt_system();
}
