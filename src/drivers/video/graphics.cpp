void SetPixel(uint32_t x, uint32_t y, int color)
{
    uint8_t* where = (uint8_t*)(y * pitch + (x * (bpp/8)) + (uint32_t)framebuffer_addr);
    where[0] = color & 255;              // BLUE
    where[1] = (color >> 8) & 255;   // GREEN
    where[2] = (color >> 16) & 255;  // RED
}

void SetPixel(int x, int y, int color)
{
    uint8_t * where = (uint8_t*)(y * pitch + (x * (bpp/8)) + (uint32_t)framebuffer_addr);
    where[0] = color & 255;              // BLUE
    where[1] = (color >> 8) & 255;   // GREEN
    where[2] = (color >> 16) & 255;  // RED
}

void draw_line_up(int x, int y, int l, int c)
{
    for (int z = y; z < y - l; z++)
        SetPixel(x, z, c);
}

void draw_line_down(int x, int y, int l, int c)
{
    for (int z = y; z < y + l; z++)
        SetPixel(x, z, c);
}

void draw_line_right(int x, int y, int l, int c)
{
    for (int z = x; z < x + l; z++)
        SetPixel(z, y, c);
}

void draw_line_left(int x, int y, int l, int c)
{
    for (int z = x; z < x - l; z++)
        SetPixel(z, y, c);
}

void draw_rect(int x, int y, int w, int h, int c)
{
    for (int z = x; z < x + w; z++)
        for (int b = y; b < y + h; b++)
            SetPixel(z, b, c);
}

void draw_empty_rect(int x, int y, int w, int h, int c)
{
    for (int z = x; z < x + w; z++)
        SetPixel(z, y, c);

    for (int z = y; z < y + h; z++)
        SetPixel(x, z, c);

    for (int z = x; z < x + w; z++)
        SetPixel(z, y+h, c);

    for (int z = y; z < y + h; z++)
        SetPixel(x+w, z, c);

    SetPixel(x+w, y+h, c);
}

void clearVideo()
{
    for (int z = 0; z < width; z++)
        for (int b = 0; b < height; b++)
            SetPixel(z, b, 0);
}
