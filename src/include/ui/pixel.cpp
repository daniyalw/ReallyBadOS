void put_pixel(int x, int y, int color)
{
    i = (x * 80 + y);
    short *vidmem = (short *) 0xb8000;

    vidmem[i] = color | ' ';
}
