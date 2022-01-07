#include <panic.h>
#include <stdio.h>
#include <kernel/interrupts.h>
#include <kernel/halt.h>
#include <kernel/log.h>

void panic(char * msg, char * file, char * function, int line)
{
    Kernel::system_log("\n\nPANIC:\n\tMessage: %s\n\n\tFile: %s\n\tFunction: %s\n\tLine: %d\n\n", msg, file, function, line);
    printf("\n\nPANIC:\n\tMessage: %s\n\n\tFile: %s\n\tFunction: %s\n\tLine: %d\n\n", msg, file, function, line);

    Kernel::disable_interrupts();
    Kernel::halt_system();
}
