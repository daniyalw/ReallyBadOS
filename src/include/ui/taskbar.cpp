#include "taskbar.h"

void taskbar(int lines)
{
    short *vidmem = (short *) 0xb8000;
    int last = 0;
    int green = 0x2a00;

    while (vidmem[last] != '\0')
    {
        if (last > lines * 80)
        {

            for (int x = 0; x < 80; x++)
            {
                put_pixel(lines, last, green);
                last++;
            }

            return;
        }

        last++;
    }
}
