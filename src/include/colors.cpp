#pragma once
#include "colors.h"

// split a color into red green and blue
colors_t get_colors(int color)
{
    colors_t colors;
    colors.blue = color & 255;
    colors.green = (color >> 8) & 255;
    colors.red = (color >> 16) & 255;
    return colors;
}

// mix as in red + black = dark red
int mix(int color1, int color2)
{
    colors_t c1, c2;
    int r, g, b;
    c1 = get_colors(color1);
    c2 = get_colors(color2);
    r = (c1.red + c2.red)/2;
    g = (c1.green + c2.green)/2;
    b = (c1.blue + c2.blue)/2;
    return get_color(r, g, b);
}

// add red and green and blue to form one color
int get_color(int red, int green, int blue)
{
    return ((red & 0xff) << 16) + ((green & 0xff) << 8) + (blue & 0xff);
}

// basically do the same thing as above
int rgb(int red, int green, int blue)
{
    return get_color(red, green, blue);
}

// common colors
int blue = rgb(0, 0, 255);
int green = rgb(0, 255, 0);
int red = rgb(255, 0, 0);
int black = 0;
int white = 0xffffff;
int yellow = 0xffff00;
int lime = 0x00ff00;
int purple = 0x800080;
int pink = 0xffc0cb;
