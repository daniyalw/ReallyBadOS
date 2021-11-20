#pragma once

void panic(char * message, char * file, int line)
{
    char * data;
    itoa(line, data);
    system_log("Kernel panic: in file ");
    system_log(file);
    system_log(", line ");
    system_log(data);
    system_log(":\n\t");
    system_log(message);
    system_log("\n");
    short * vidmem = (short *)0xb8000;
    int bg = 0x1F00; // blue

    // make background blue
    for (int z = 0; z < 80*25; z++)
        vidmem[z] = bg | ' ';

    text_x = 0;
    text_y = 0;

    // display error message
    printf(bg, "Kernel panic: in file %s, line %d:\n\t%s", file, line, message);

    asm("cli");
    asm("hlt");
    while (true);
}
