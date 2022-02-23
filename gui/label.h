#pragma once

#include <gui/coords.h>
#include <gui/widget.h>

typedef widget_t label_t;

label_t create_label(auto win, char *text, int x, int y, int wrap, int bg, int fg);
label_t create_label(auto win, char *text, int x, int y, int wrap);
void draw_label(widget_t label, coords_t coords);
