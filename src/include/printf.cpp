#include "printf.h"

int printf(char * mystring)
{
    short *vidmem = (short *) 0xb8000;
    int last = 0;
    int white = 0x0F00;
    int limit = 80 * 25;

    while (mystring[last] != '\0')
    {
        if (mystring[last] == '\n') {
            printed += '\n';
            y += 1;
            x = 0;
        }
        else {
            vidmem[y*80+x] = color | mystring[last];
            printed += mystring[last];
            x++;
        }
        last++;

    }

    update_cursor();

    return 0;
}

int print(char * mystring, int loc, int bg)
{
    short *vidmem = (short *) 0xb8000;

    for (int x = 0; x < len(mystring); x++)
        vidmem[loc+x] = color | mystring[x];

    return 0;
}
