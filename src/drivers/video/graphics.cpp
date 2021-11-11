#pragma once
#include "graphics.h"
#include "font.cpp"

void center_printf(char * string, int x, int y, int w);

void SetPixel(int x, int y, int color)
{
    back_buffer[y*width+x] = color;
    uint8_t * where = (uint8_t*)(y * pitch + (x * (bpp/8)) + (uint32_t)framebuffer_addr);
    where[0] = color & 255;              // BLUE
    where[1] = (color >> 8) & 255;   // GREEN
    where[2] = (color >> 16) & 255;  // RED
}

void show_buffer()
{
    int color;

    for (int z = 0; z < width; z++) {
        for (int b = 0; b < height; b++) {
            color = back_buffer[b*width+z];
            uint8_t * where = (uint8_t*)(b * pitch + (z * (bpp/8)) + (uint32_t)framebuffer_addr);
            where[0] = color & 255;              // BLUE
            where[1] = (color >> 8) & 255;   // GREEN
            where[2] = (color >> 16) & 255;  // RED
        }
    }
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

void draw_triangle(int xc, int yc, int r, int color)
{
    int x = xc - r;
    int y = yc - r;
    int xd = 1;
    int f1 = 1;
    int f2 = 1;

    for (int i = y; i < y + r * 2; i++)
    {
        f1 = x + r - xd;
        f2 = x + r + xd;
        xd++;
        for (int z = f1; z < f2; z++)
        {
            SetPixel(z, i, color);
        }
    }
}

void __draw_circle__(int xc, int yc, int x, int y, uint32_t color)
{
    SetPixel(xc + x, yc + y, color);
    SetPixel(xc - x, yc + y, color);
    SetPixel(xc + x, yc - y, color);
    SetPixel(xc - x, yc - y, color);
    SetPixel(xc + y, yc + x, color);
    SetPixel(xc - y, yc + x, color);
    SetPixel(xc + y, yc - x, color);
    SetPixel(xc - y, yc - x, color);
}

void draw_circle(int x_center, int y_center, int r, uint32_t color)
{
    int x = 0, y = r, d = 3 - (2 * r);

    __draw_circle__(x_center, y_center, x, y, color);

    while(y >= x)
    {
        x++;

        if(d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        __draw_circle__(x_center, y_center, x, y, color);
    }
}

void create_shutdown_button()
{
    int color;
    draw_rect(0, 0, 20, 20, rgb(255, 0, 0));
    draw_line_down(10, 2, 8, 0);
    draw_line_right(5, 5, 3, 0);
    draw_line_right(13, 5, 3, 0);
    draw_line_down(5, 5, 10, 0);
    draw_line_right(5, 15, 11, 0);
    draw_line_down(16, 5, 11, 0);
}
