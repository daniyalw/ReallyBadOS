#include "taskbar.h"

void taskbar(int lines)
{
    short *vidmem = (short *) 0xb8000;
    int last = 0;
    int _do = lines * 80;

    for (int x = 0; x < _do -1; x++)
    {
        vidmem[x] = bg | ' ';
    }

    while (vidmem[last] != '\0')
    {
        if (last > _do)
        {
            vidmem[last-1] = blue | ' ';
            last = _do;

            for (int x = 0; x < 81*3; x++)
            {
                vidmem[last-81*3+3] = blue | ' ';
                last++;
            }

            return;
        }

        last++;
    }
}
