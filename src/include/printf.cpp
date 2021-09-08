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
            vidmem[i] = bg | mystring[last];
        }
        i++;
        last++;

    }

    update_cursor();

    return 0;
}

int print(char * mystring, int loc, int bg)
{
    short *vidmem = (short *) 0xb8000;

    for (int x = 0; x < len(mystring); x++)
        vidmem[loc+x] = bg | mystring[x];

    return 0;
}
