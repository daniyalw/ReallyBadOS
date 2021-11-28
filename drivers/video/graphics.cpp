#pragma once
#include "graphics.h"
#include "font.cpp"

void center_printf(char * string, int x, int y, int w);

void SetPixel(int x, int y, int color)
{
    back_buffer[y*width+x] = color;
    uint8_t * where = (uint8_t*)(y * pitch + (x * (bpp/8)) + (int)framebuffer_addr);
    where[0] = color & 255;              // BLUE
    where[1] = (color >> 8) & 255;   // GREEN
    where[2] = (color >> 16) & 255;  // RED
}

void redraw_background_picture(int list[])
{
    int color;

    for (int z = 0; z < 768; z++)
    {
        for (int b = 0; b < 1024; b++)
        {
            color = list[b+z*1024];
            SetPixel(b, z, color);
        }
    }
}

void redraw()
{
    int color;

    for (int z = 0; z < width; z++) {
        for (int b = 0; b < height; b++) {
            color = back_buffer[b*width+z];
            uint8_t * where = (uint8_t*)(b * pitch + (z * (bpp/8)) + (int)framebuffer_addr);
            where[0] = color & 255;              // BLUE
            where[1] = (color >> 8) & 255;   // GREEN
            where[2] = (color >> 16) & 255;  // RED
        }
    }
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

void clear_screen()
{
    for (int z = 0; z < width; z++)
        for (int b = 0; b < height; b++)
            SetPixel(z, b, 0);
}

void center_printf(char * string, int x, int y, int w)
{
    g_printf(string, (w/2)+x, y);
}
