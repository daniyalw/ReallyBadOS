void panic(char * message, char * options)
{
    // can't use printf because it'll give a a black background
    // therefore we must write to the memory
    short *vidmem = (short *) 0xb8000;
    char * panic_message = "Kernel panic: ";
    char * op = "\nOptions:\n";
    char * dash = "\n - ";
    char * newline = "\n";
    int last = 1;
    char * empty = "  ";
    int z = 0;
    int b = 0;
    int a = 0;
    int d = 0;
    printed = "";
    x = 0;
    y = 0;

    for (int z = 0; z < 80 * 25; z++)
    {
        vidmem[z] = panic_color | empty[0];
    }

    vidmem[0] = panic_color | panic_message[0];

    while (panic_message[last] != '\0')
    {
        vidmem[last] = panic_color | panic_message[last];
        printed += panic_message[last];
        last++;
        x++;
    }

    while (message[z] != '\0')
    {
        vidmem[last] = panic_color | message[z];
        printed += message[z];
        last++;
        z++;
        x++;
    }

    for (int v = 0; v < len(op); v++)
    {
        if (op[v] == '\n')
        {
            y += 80;
            x = 0;

            printed += newline[0];
        }
        else
        {
            vidmem[y*80+x] = panic_color | op[v];
            printed += op[v];
        }
        last++;
        x++;
    }

    for (int d = 0; d < len(options); d++)
    {
        if (options[d] == '\n')
        {
            for (int n = 0; n < len(dash); n++)
            {
                if (dash[n] == '\n')
                {
                    y += 80;
                    x = 0;
                    printed += newline[0];
                }
                else
                {
                    vidmem[y*80+x] = panic_color | dash[n];
                    x++;
                    printed += dash[n];
                }
                last++;
            }

            for (int n = 0; n < (b - a); n++)
            {
                vidmem[y*80+x] = panic_color | options[n];
                last++;
                x++;
                printed += options[n];
            }

            a = d;
        }
        b++;
        last++;
    }
}
