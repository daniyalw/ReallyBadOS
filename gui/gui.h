#pragma once

#define MAX_ 128

#include <colors.h>

class Widget;
class Window;

class Widget
{
protected:
    int id;
    int window_id;
public:
    int x, y, w, h;
    int color;
    Widget();
    void start(int win_id);
    void draw_widget() {};
    void hide_widget() {};
    bool active = false;
};
/*
class Canvas : protected Widget
{
public:
    Canvas()
    {};
    void draw_widget()
    {};
    void hide_widget()
    {};
};
*/
class Window
{
    int id;
public:
    Widget * widgets[MAX_];
    int widget_count = 0;
    int w, h;
    int z;
    bool active = false;
    int avx, avy;
    int bg = Graphic::rgb(100, 100, 100);
    Window();
    int get_id();
    void draw_window();
};

std::list<Window *> windows;

void save_widget(Widget widget);
void save_window(Window win);
