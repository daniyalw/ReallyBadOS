#pragma once
#include <kernel/power.h>
#include <kernel/io.h>

//https://wiki.osdev.org/Reboot

namespace Kernel {

void reboot()
{
    asm("cli");
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    asm("hlt");
}

}
