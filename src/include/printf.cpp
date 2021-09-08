#include "printf.h"

int printf(char * mystring)
{
    short *vidmem = (short *) 0xb8000;
    int last = 0;
    int white = 0x0F00;

    while (mystring[last] != '\0')
    {
        if (mystring[last] == '\n') {
            i += 80;
        }
        else {
            vidmem[i] = white | mystring[last];
        }
        i++;
        last++;

    }

    update_cursor();

    return 0;
}
