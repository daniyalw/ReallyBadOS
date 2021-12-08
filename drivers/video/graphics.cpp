#pragma once
#include <drivers/video/graphics.h>
#include <colors.h>
#include "font.cpp"

namespace Graphic {

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
            Graphic::SetPixel(b, z, color);
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
        Graphic::SetPixel(z, y, c);

    for (int z = y; z < y + h; z++)
        Graphic::SetPixel(x, z, c);

    for (int z = x; z < x + w; z++)
        Graphic::SetPixel(z, y+h, c);

    for (int z = y; z < y + h; z++)
        Graphic::SetPixel(x+w, z, c);

    Graphic::SetPixel(x+w, y+h, c);
}

void clear_screen()
{
    for (int z = 0; z < width; z++)
        for (int b = 0; b < height; b++)
            Graphic::SetPixel(z, b, 0);
}

void center_printf(char * string, int x, int y, int w)
{
    g_printf(string, (w/2)+x, y);
}

void topleft_plotpoints(int x, int y, int cx, int cy, int color) {
    Graphic::SetPixel(cx - x, cy - y, color);
    Graphic::SetPixel(cx - y, cy - x, color);
}

void topright_plotpoints(int x, int y, int cx, int cy, int color) {
    Graphic::SetPixel(cx + x, cy - y, color);
    Graphic::SetPixel(cx + y, cy - x, color);
}

void bottomright_plotpoints(int x, int y, int cx, int cy, int color) {
    Graphic::SetPixel(cx + x, cy + y, color);
    Graphic::SetPixel(cx + y, cy + x, color);
}

void bottomleft_plotpoints(int x, int y, int cx, int cy, int color) {
    Graphic::SetPixel(cx - x, cy + y, color);
    Graphic::SetPixel(cx - y, cy + x, color);
}

void full_plotpoints(int x, int y, int cx, int cy, int color) {
    Graphic::SetPixel(cx + x, cy + y, color);
    Graphic::SetPixel(cx - x, cy + y, color);
    Graphic::SetPixel(cx + x, cy - y, color);
    Graphic::SetPixel(cx - x, cy - y, color);
    Graphic::SetPixel(cx + y, cy + x, color);
    Graphic::SetPixel(cx - y, cy + x, color);
    Graphic::SetPixel(cx + y, cy - x, color);
    Graphic::SetPixel(cx - y, cy - x, color);
}

void topleft_circle(int cx, int cy, int r, int color) {
    int x = 0, y, p;
    y = r;
    p = 1 - r;

    while (x < y) {
        topleft_plotpoints(x, y, cx, cy, color);
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else {
                y--;
                p += 2 * (x - y) + 1;
        }
    }
}

void topright_circle(int cx, int cy, int r, int color) {
    int x = 0, y, p;
    y = r;
    p = 1 - r;

    while (x < y) {
        topright_plotpoints(x, y, cx, cy, color);
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else {
                y--;
                p += 2 * (x - y) + 1;
        }
    }
}

void bottomleft_circle(int cx, int cy, int r, int color) {
    int x = 0, y, p;
    y = r;
    p = 1 - r;

    while (x < y) {
        bottomleft_plotpoints(x, y, cx, cy, color);
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else {
                y--;
                p += 2 * (x - y) + 1;
        }
    }
}

void bottomright_circle(int cx, int cy, int r, int color) {
    int x = 0, y, p;
    y = r;
    p = 1 - r;

    while (x < y) {
        bottomright_plotpoints(x, y, cx, cy, color);
        x++;
        if (p < 0)
            p += 2 * x + 1;
        else {
                y--;
                p += 2 * (x - y) + 1;
        }
    }
}

void circle(int cx, int cy, int r, int color) {
    bottomleft_circle(cx, cy, r, color);
    bottomright_circle(cx, cy, r, color);
    topleft_circle(cx, cy, r, color);
    topright_circle(cx, cy, r, color);
}

void fill_circle(int cx, int cy, int radius, int color)
{
    circle(cx, cy, radius, color);

    for(int y = -radius; y <= radius; y++)
        for(int x = -radius; x <= radius; x++)
            if(x * x + y * y <= radius * radius)
                Graphic::SetPixel(cx+x, cy+y, color);
}

void rounded_rectangle(int x, int y, int w, int h, int r, int color)
{
    // draw rounded corners
    topleft_circle(x+r, y+r, r, color);
    topright_circle((x+w)-r, y+r, r, color);
    bottomright_circle((x+w)-r, (y+h)-r, r, color);
    bottomleft_circle(x+r, (y+h)-r, r, color);

    // draw the lines in between the corners
    for (int z = x + r; z < (x + w) - r; z++)
        SetPixel(z, y, color);

    for (int b = y + r; b < (y + h) - r; b++)
        SetPixel(x, b, color);

    for (int b = y + r; b < (y + h) - r; b++)
        SetPixel(x + w, b, color);

    for (int z = x + r; z < (x + w) - r; z++)
        SetPixel(z, y + h, color);
}

void fill_rounded_rectangle(int x, int y, int w, int h, int r, int color)
{
    fill_circle(x+r, y+r, r, color);
    fill_circle((x+w)-r, y+r, r, color);
    fill_circle((x+w)-r, (y+h)-r, r, color);
    fill_circle(x+r, (y+h)-r, r, color);
    draw_rect(x+r, y, w-(r*2), h+1, color);
    draw_rect(x, y+r, w+1, h-(r*2), color);
}

void tint(int * arr, int w, int h, int color)
{
    for (int z = 0; z < w; z++)
    {
        for (int b = 0; b < h; b++)
        {
            arr[z+b*w] = Graphic::mix(arr[z+b*w], color);
        }
    }
}

}
