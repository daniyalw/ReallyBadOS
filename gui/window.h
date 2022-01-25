#pragma once

#include <video/graphics.h>
#include <gui/widget.h>
#include <gui/event.h>

using namespace Graphic;

#define TITLE_PADDING 2
#define CLOSE_B 6
#define RADIUS 10

class Window
{
public:
    int x, y, w, h;
    int id;
    char * title;

    int bg = rgb(255, 0, 0);

    Widget widgets[100];
    int widget_count = 0;

    Window();

    void set_title(char * t) { title = t; }

    void draw();
    void set(int _x, int _y, int _w, int _h);
    void add_widget(Widget w);
    void replace_widget(Widget w, int _id);
};

std::list<Window *> windows;
