#pragma once

typedef struct {
    int red;
    int green;
    int blue;
} colors_t;

colors_t get_colors(int color);
int mix(int color1, int color2);
int get_color(int red, int green, int blue);
int rgb(int red, int green, int blue);
