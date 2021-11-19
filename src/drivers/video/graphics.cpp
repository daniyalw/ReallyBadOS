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

void draw_horizontal_line(int color, int dx, int x1, int y1)
{
    for (int i = 0; i < dx; i++)
        SetPixel(x1 + i, y1, color);
}

void draw_vertical_line(int color, int dy, int x1, int y1)
{
    for(int i = 0; i < dy; i++)
        SetPixel(x1, y1 + i, color);
}

void draw_diagonal_line(int color, int dx, int dy, int x1, int y1)
{
    int i;
    int x, y;
    int px, py;
    int sx, sy;
    int x_abs, y_abs;

    x_abs = abs(dx);
    y_abs = abs(dy);
    sx = sign(dx);
    sy = sign(dy);
    x = y_abs >> 1;
    y = x_abs >> 1;
    px = x1;
    py = y1;

    if (x_abs >= y_abs) {
        for (i = 0; i < x_abs; i++)
        {
            y += y_abs;
            if (y >= x_abs)
            {
                y -= x_abs;
                py += sy;
            }
            px += sx;
            SetPixel(px, py, color);
        }
    } else {
        for (i = 0; i < y_abs; i++)
        {
            x += x_abs;
            if (x >= y_abs)
            {
                x -= y_abs;
                px += sx;
            }
            py += sy;
            SetPixel(px, py, color);
        }
    }
}

void draw_line(int color, int x1, int y1, int x2, int y2)
{
    int dx=x2-x1, dy=y2-y1;

    if (dy == 0)
    {
        if (dx < 0)
            draw_horizontal_line(color, abs(dx), x2, y1);
        else
            draw_horizontal_line(color, dx, x1, y1);
    }

    if (dx == 0)
    {
        if (dy < 0)
            draw_vertical_line(color, abs(dy), x1, y2);
        else
            draw_vertical_line(color, dy, x1, y1);
    }
    else
        draw_diagonal_line(color, dx, dy, x1, y1);

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

void create_shutdown_button()
{
    int color;

}

void circle_helper(int x, int y, int radius, int corner, int color)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int i = 0;
    int j = radius;

    while (i < j) {
        if (f >= 0) {
            j--;
            ddF_y += 2;
            f += ddF_y;
        }
        i++;
        ddF_x += 2;
        f += ddF_x;
        if (corner & 0x4) {
            SetPixel(x + i, y + j, color);
            SetPixel(x + j, y + i, color);
        }
        if (corner & 0x2) {
            SetPixel(x + i, y - j, color);
            SetPixel(x + j, y - i, color);
        }
        if (corner & 0x8) {
            SetPixel(x - j, y + i, color);
            SetPixel(x - i, y + j, color);
        }
        if (corner & 0x1) {
            SetPixel(x - j, y - i, color);
            SetPixel(x - i, y - j, color);
        }
    }
}

void draw_empty_circle(int x, int y, int radius, int color)
{
    circle_helper(x, y, radius, 3, color);
    circle_helper(x, y, radius, 4, color);
    circle_helper(x, y, radius, 8, color);
}

void help_fill_circle(int x, int y, int radius, int corner, int delta, int color)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int i = 0;
    int j = radius;

    while (i < j) {
        if (f >= 0) {
            j--;
            ddF_y += 2;
            f += ddF_y;
        }
        i++;
        ddF_x += 2;
        f += ddF_x;

        if (corner & 0x1) {
            draw_vertical_line(color, 2 * j + 1 + delta, x + i, y - j);
            draw_vertical_line(color, 2 * i + 1 + delta, x + j, y - i);
        }
        if (corner & 0x2) {
            draw_vertical_line(color, 2 * j + 1 + delta, x - i, y - j);
            draw_vertical_line(color, 2 * i + 1 + delta, x - j, y - i);
        }
        if (corner & 0x4) {
            draw_horizontal_line(color, 2 * j + 1 + delta, x + i, y + j);
            draw_horizontal_line(color, 2 * i + 1 + delta, x + j, y + i);
        }
    }
}

void draw_fill_rect(int color, int x_start, int y_start, int width, int height)
{
    for (int y = y_start; y < y_start + height; y++)
    {
        draw_line(color, x_start, y, x_start + width, y);
    }
}

void draw_rounded_rect(int color, int x, int y, int width, int height, int radius)
{
    draw_fill_rect(color, x + radius, y, width - 2 * radius + 1, height);

    help_fill_circle(x + width - radius - 1, y + radius, radius, 1, height - 2 * radius - 1, color);
    help_fill_circle(x + radius, y + radius, radius, 2, height - 2 * radius - 1, color);
}

void draw_circle(int color, int x, int y, int radius)
{
    draw_vertical_line(color, 2 * radius + 1, x, y - radius);
    help_fill_circle(x, y, radius, 3, 0, color);
}
