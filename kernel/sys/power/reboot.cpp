#pragma once

//https://wiki.osdev.org/Reboot

void reboot()
{
    asm("cli");
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
    asm("hlt");
}
