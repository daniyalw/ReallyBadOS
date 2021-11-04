void panic(char * text)
{
    int last = 0;
    int x = 0;
    int y = 0;
    short * vidmem = (short *)0xb8000;
    char * message = "Kernel panic: ";
    int color = 0x1F00; // white-on-blue

    for (int z = 0; z < 80 * 25; z++)
    {
        vidmem[z] = color | ' ';
    }

    while (message[last] != '\0')
    {
        if (message[last] == '\n')
        {
            x = 0;
            y++;
        }
        else
        {
            vidmem[y*80+x] = color | message[last];
            x++;

            if (x > 80)
            {
                x = 0;
                y++;
            }
        }
        last++;
    }

    last = 0;

    while (text[last] != '\0')
    {
        if (text[last] == '\n')
        {
            x = 0;
            y++;
        }
        else
        {
            vidmem[y*80+x] = color | text[last];
            x++;

            if (x > 80)
            {
                x = 0;
                y++;
            }
        }
        last++;
    }

    // shut down everything
    asm volatile("cli"); // disable interrupts
    asm volatile("hlt"); // halt
}
