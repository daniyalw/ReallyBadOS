int clear()
{
    short *vidmem = (short *) 0xb8000;
    int last = 0;
    int white = 0x0F00;

    while (vidmem[last] != '\0')
    {
        vidmem[last] = white | ' ';
        last++;
    }

    video_x = 0;
    video_y = 0;

    return 0;
}
