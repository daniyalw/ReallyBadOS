#pragma once

#include <gui/coords.h>

typedef struct widget_t
{
    int id;
    void *data[10];
    int *extra[10];
    int parent_id;

    int bg, fg;

    bool to_draw = false;
    bool active = false;

    coords_t coords;

    void (*mouse_click)(int, widget_t, coords_t, bool, bool, bool);
    void (*key_click)(widget_t, char);
    void (*draw_widget)(widget_t, coords_t);

    auto draw();
    auto hide();
} widget_t;

void default_mouse_click(int win_id, widget_t widget, coords_t coords, bool right, bool left, bool middle) {}

void default_key_click(widget_t widget, char key) {}

void default_draw(widget_t widget, coords_t coords) {}

auto add_widget(auto win, widget_t widget);
auto widget_save(auto win, widget_t widget);

auto to_widget_draw_hide(widget_t *_widget);
