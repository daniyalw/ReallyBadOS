#pragma once

#include <gui/widget.h>

typedef widget_t entry_t;

void handle_entry_key(entry_t entry, char key);
void draw_entry(entry_t entry, coords_t coords);
entry_t create_entry(auto win, int x, int y, int bg, int fg);
entry_t create_entry(auto win, int x, int y);
