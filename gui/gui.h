#pragma once

#define MAX_ 128

#include <colors.h>

struct Widget
{
    int window_id;
    int id;
    int x, y, w, h;
    int color;
    void draw_widget();
    void hide_widget();
    bool active = false;
};

struct Window
{
    int id;
    Widget widgets[MAX_];
    int widget_count = 0;
    int z;
    bool active = false;
    int avx, avy;
    int bg = Graphic::rgb(100, 100, 100);
    void draw_window();
};

std::list<Window> windows;

Window create_window();
Widget create_widget(Window win);
