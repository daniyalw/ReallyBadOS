#pragma once
#include <drivers/video/graphics.h>
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

void blit_changes()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int color = back_buffer[y*width+x];
            uint8_t * where = (uint8_t*)(y * pitch + (x * (bpp/8)) + (int)framebuffer_addr);
            where[0] = color & 255;              // BLUE
            where[1] = (color >> 8) & 255;   // GREEN
            where[2] = (color >> 16) & 255;  // RED
        }
    }
}

void redraw_background_picture(int list[])
{
    int color;

    for (int z = 0; z < height; z++)
    {
        for (int b = 0; b < width; b++)
        {
            color = list[b+z*width];
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
        {
            back_buffer[b*width+z] = c;
            uint8_t * where = (uint8_t*)(b * pitch + (z * (bpp/8)) + (int)framebuffer_addr);
            where[0] = c & 255;              // BLUE
            where[1] = (c >> 8) & 255;   // GREEN
            where[2] = (c >> 16) & 255;  // RED
        }
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


void plotpoints(int x, int y, int cx, int cy, int color) {
    SetPixel(cx + x, cy + y, color);
    SetPixel(cx - x, cy + y, color);
    SetPixel(cx + x, cy - y, color);
    SetPixel(cx - x, cy - y, color);
    SetPixel(cx + y, cy + x, color);
    SetPixel(cx - y, cy + x, color);
    SetPixel(cx + y, cy - x, color);
    SetPixel(cx - y, cy - x, color);
}

void circle(int cx, int cy, int r, int color) {
    int x = 0, y, p;
    y = r;
    p = 1 - r;

    while (x < y) {
        plotpoints(x, y, cx, cy, color);
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else {
                y--;
                p += 2 * (x - y) + 1;
        }
    }
}

void fill_circle(int cx, int cy, int radius, int color)
{
    circle(cx, cy, radius, color);

    for(int y = -radius; y <= radius; y++)
        for(int x = -radius; x <= radius; x++)
            if(x * x + y * y <= radius * radius)
                SetPixel(cx+x, cy+y, color);
}

void rounded_rectangle(int x, int y, int w, int h, int r, int color)
{
    fill_circle(x+r, y+r, r, color);
    fill_circle((x+w)-r, y+r, r, color);
    fill_circle((x+w)-r, (y+h)-r, r, color);
    fill_circle(x+r, (y+h)-r, r, color);
    draw_rect(x+r, y, w-(r*2), h+1, color);
    draw_rect(x, y+r, w+1, h-(r*2), color);
}
