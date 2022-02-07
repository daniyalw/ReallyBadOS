#pragma once

#include <gui/widget.h>
#include <video/graphics.h>
#include <fs.h>

struct Window
{
    int x, y, w, h;
    int bg = rgb(100, 100, 100);

    int title_x, title_y;
    char title[100];

    Widget widgets[100];
    int widget_count = 0;

    int id;

    void draw();
    void set_coords(int _x, int _y) { x = _x; y = _y; }

    void set_dimensions(int _w, int _h) { w = _w; h = _h; }
    void set_title(char *_title) { for (int z = 0; z < 100; z++) title[z] = 0; for (int z = 0; z < len(_title); z++) title[z] = _title[z];}

    void add_widget(Widget widget);

    void set_id(int _id) { id = _id; }
};

Window windows[100];
int window_count = 0;
