#pragma once
#include <sys/power/power.h>
#include <sys/io.h>

//https://wiki.osdev.org/Reboot

namespace Kernel {

void reboot() {
    log::warning("Rebooting...");

    asm("cli");
    uint8_t good = 0x02;
    while (good & 0x02)
        good = Kernel::IO::inb(0x64);
    Kernel::IO::outb(0x64, 0xFE);
    asm("hlt");
}

}
