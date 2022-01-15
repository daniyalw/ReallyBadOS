#pragma once
#include <drivers/video/graphics.h>
#include <colors.h>
#include <font.h>
#include <math.h>

namespace Graphic {

void center_printf(char * string, int x, int y, int w);

inline void SetPixel(int x, int y, int color)
{
    back_buffer[y*width+x] = color;

    if (blit_right_away)
    {
        uint8_t * where = (uint8_t*)(y * pitch + (x * (bpp/8)) + (int)framebuffer_addr);
        where[0] = color & 255;              // BLUE
        where[1] = (color >> 8) & 255;   // GREEN
        where[2] = (color >> 16) & 255;  // RED
    }
}

inline void blit_changes()
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

inline void redraw_background_picture(int list[])
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

inline void draw_rect(int x, int y, int w, int h, int c)
{
    for (int z = x; z < x + w; z++)
        for (int b = y; b < y + h; b++)
        {
            back_buffer[b*width+z] = c;

            if (blit_right_away) {
                uint8_t * where = (uint8_t*)(b * pitch + (z * (bpp/8)) + (int)framebuffer_addr);
                where[0] = c & 255;              // BLUE
                where[1] = (c >> 8) & 255;   // GREEN
                where[2] = (c >> 16) & 255;  // RED
            }
        }
}

// this function gets the pixels from the array in that same location to be displayed on-screen
//
// so for location (10, 30) it would get the color at location 10 + 30 * width in the array
inline void draw_rect(int x, int y, int w, int h, int *arr)
{
    for (int z = x; z < x + w; z++)
    {
        for (int b = y; b < y + h; b++)
        {
            int c = arr[z+b*width];
            back_buffer[b*width+z] = c;

            if (blit_right_away) {
                uint8_t * where = (uint8_t*)(b * pitch + (z * (bpp/8)) + (int)framebuffer_addr);
                where[0] = c & 255;              // BLUE
                where[1] = (c >> 8) & 255;   // GREEN
                where[2] = (c >> 16) & 255;  // RED
            }
        }
    }
}

// this one gets the pixels from the array from the start
inline void draw_rect_arr(int x, int y, int w, int h, int *arr)
{
    for (int z = 0; z < w; z++)
    {
        for (int b = 0; b < h; b++)
        {
            int c = arr[z+b*width];

            if (c == -1) // -1 is just transparent
            {
                c = back_buffer[(z + x) + (b + y) * width];
            }
            else
            {
                back_buffer[(z + x)+(b + y)*width] = c;
            }

            if (blit_right_away)
            {
                uint8_t * where = (uint8_t*)((b + y) * pitch + ((z + x) * (bpp/8)) + (int)framebuffer_addr);
                where[0] = c & 255;              // BLUE
                where[1] = (c >> 8) & 255;   // GREEN
                where[2] = (c >> 16) & 255;  // RED
            }
        }
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

inline void line_low(int x0, int y0, int x1, int y1, int color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;

    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y0;

    for (int x = x0; x < x1; x++)
    {
        SetPixel(x, y, color);
        if (D > 0)
        {
            y = y + yi;
            D = D + (2 * (dy - dx));
        }
        else
        {
            D = D + 2*dy;
        }
    }
}

inline void line_high(int x0, int y0, int x1, int y1, int color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = x0;

    for (int y = y0; y < y1; y++)
    {
        SetPixel(x, y, color);
        if (D > 0)
        {
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else
        {
            D = D + 2*dx;
        }
    }
}

inline void draw_line(int x0, int y0, int x1, int y1, int color)
{
    if (Math::abs(y1 - y0) < Math::abs(x1 - x0))
    {
        if (x0 > x1)
            line_low(x1, y1, x0, y0, color);
        else
            line_low(x0, y0, x1, y1, color);
    }
    else
    {
        if (y0 > y1)
            line_high(x1, y1, x0, y0, color);
        else
            line_high(x0, y0, x1, y1, color);
    }
}

void center_text_graphics(char * string, int x, int y, int w, int color)
{
    draw_string(string, x+((w - std::len(string) * 8)/2), y, color);
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

void fill_topleft_corner(int cx, int cy, int radius, int color, int position) {
    for (int y = -radius; y <= 0; y++)
        for (int x = -radius; x <= 0; x++)
            if (x * x + y * y <= radius * radius)
                Graphic::SetPixel(cx+x, cy+y, color);
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

    for (int y = -radius; y <= radius; y++)
        for (int x = -radius; x <= radius; x++)
            if (x * x + y * y <= radius * radius)
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
