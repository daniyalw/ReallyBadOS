#pragma once
#include <string.h>
#include <font.h>

/*
skiftOS

Copyright © 2018-2021 N. Van Bossuyt & contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/

void font_draw(char cp, int x, int y, int fg, int bg);

void font_draw(char cp, int x, int y);
void font_draw_double(char cp, int x, int y, int fg, int bg);
void font_draw_double(char cp, int x, int y);

void font_set_fg(int color) {
    fg = color;
}

void font_set_bg(int color) {
    bg = color;
}

void g_putchar(char ch, int x, int y)
{
    char no;

    for (int z = 0; z < 128; z++)
        if (font_ascii[z] == ch)
        {
            no = (char)z;
            break;
        }

    font_draw(no, x, y);
}

void g_putchar(char ch, int x, int y, int fg, int bg)
{
    char no;

    for (int z = 0; z < 128; z++)
        if (font_ascii[z] == ch)
        {
            no = (char)z;
            break;
        }

    font_draw(no, x, y, fg, bg);
}

void g_printf(char * string, int x, int y, int fg, int bg)
{
    int xx = x;
    int yy = y;

    for (int z = 0; z < std::len(string); z++)
    {
        g_putchar(string[z], xx, yy, fg, bg);
        xx += 9;
    }
}

void g_printf(char * string, int x, int y)
{
    int xx = x;
    int yy = y;

    for (int z = 0; z < std::len(string); z++)
    {
        g_putchar(string[z], xx, yy);
        xx += 9;
    }
}

void font_draw(char cp, int x, int y)
{
    if (cp > 0x7f)
        return;

    for (int yy = 0; yy < 9; yy++)
    {
        for (int xx = 0; xx < 9; xx++)
        {
            bool set = xx != 8 && yy != 8 && (font8x8_basic[cp][yy] & (1 << xx));

            if (set)
                Graphic::SetPixel(x + xx, y + yy, fg);
        }
    }
}

void font_draw(char cp, int x, int y, int fg, int bg)
{
    if (cp > 0x7f)
        return;

    for (int yy = 0; yy < 9; yy++)
    {
        for (int xx = 0; xx < 9; xx++)
        {
            bool set = xx != 8 && yy != 8 && (font8x8_basic[cp][yy] & (1 << xx));

            if (set)
            {
                Graphic::SetPixel(x + xx, y + yy, fg);
            }
            else
            {
                Graphic::SetPixel(x + xx, y + yy, bg);
            }
        }
    }
}

void font_draw_double(char cp, int x, int y, int fg, int bg)
{
    if (cp > 0x7f)
        return;

    int scale = 2;

    for (int yy = 0; yy < 17; yy++)
    {
        for (int xx = 0; xx < 17; xx++)
        {
            bool set = xx != 16 && yy != 16 && ((uint8_t)(font8x8_basic[cp][yy/scale]) & (1 << xx));

            if (set)
            {
                Graphic::SetPixel(x + xx, y + yy, fg);
            }
            else
            {
                Graphic::SetPixel(x + xx, y + yy, bg);
            }
        }
    }
}

void font_draw_double(char cp, int x, int y)
{
    if (cp > 0x7f)
        return;

    int scale = 2;

    for (int yy = 0; yy < 17; yy++)
    {
        for (int xx = 0; xx < 17; xx++)
        {
            bool set = xx != 16 && yy != 16 && ((uint8_t)(font8x8_basic[cp][yy/scale]) & (1 << xx));

            if (set)
                Graphic::SetPixel(x + xx, y + yy, fg);
        }
    }
}
