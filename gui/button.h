#pragma once

#define BUTTON_PIXEL_PADDING 3

#include <gui/coords.h>
#include <gui/widget.h>

typedef widget_t button_t;

void get_mouse_click(button_t button, coords_t coords);
void draw_button(button_t button, coords_t coords);
button_t create_button(auto win, char *text, int x, int y, auto callback, int bg, int fg);
button_t create_button(auto win, char *text, int x, int y, auto callback);
