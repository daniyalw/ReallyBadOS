#include "printf.h"

int printf(char * mystring)
{
    short *vidmem = (short *) 0xb8000;
    int last = 0;
    int white = 0x0F00;
    int limit = 80 * 25;
    char * copydata;
    int z = 0;

    while (mystring[last] != '\0')
    {
        if (last > limit)
        {
            while (printed[z] != '\0')
            {
                if (z > 80)
                    copydata += printed[z];

                z++;
            }
            clear();
            printf(copydata);
            copydata = (char *)' ';
            printed = (char *)' ';
            z = 0;
        }
        if (mystring[last] == '\n') {
            printed += '\n';
            video_y += 1;
            video_x = 0;
        }
        else {
            vidmem[video_y*80+video_x] = color | mystring[last];
            printed += mystring[last];
            video_x++;
        }
        last++;

    }

    update_cursor();

    return 0;
}

int print(char mystring, int loc, int bg)
{
    short *vidmem = (short *) 0xb8000;

    for (int x = 0; video_x < len(mystring); x++)
        vidmem[loc+x] = color | mystring[x];

    return 0;
}
