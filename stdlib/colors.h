#pragma once

typedef struct {
    int red;
    int green;
    int blue;
} colors_t;

namespace Graphic {

colors_t get_colors(int color);
int mix(int color1, int color2);
int get_color(int red, int green, int blue);
int rgb(int red, int green, int blue);

}

// common colors
int blue = Graphic::rgb(0, 0, 255);
int green = Graphic::rgb(0, 255, 0);
int red = Graphic::rgb(255, 0, 0);
int black = 0;
int white = 0xffffff;
int yellow = 0xffff00;
int lime = 0x00ff00;
int purple = 0x800080;
int pink = 0xffc0cb;
