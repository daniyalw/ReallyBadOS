#pragma once

#include "gui.h"
#include <colors.h>

Window create_window()
{
    Window win;
    win.id = windows.size();
    win.avx = 10;
    win.avy = 10;
    win.z = 0;
    windows.insert_at(win, win.z);
    return win;
}

void Window::draw_window()
{
    for (int z = 0; z < width; z++)
        for (int b = 0; b < height; b++)
            Graphic::SetPixel(z, b, bg);

    active = true;

    Window w = windows.get(id);
    w.active = active;
    windows.replace(w, id);

    for (int z = 0; z < widget_count; z++)
    {
        if (this->widgets[z].active)
            widgets[z].draw_widget();
    }
}

void Widget::draw_widget()
{
    if (windows.get(window_id).active)
        Graphic::draw_rect(x, y, w, h, color);

    active = true;

    Window w = windows.get(window_id);
    w.widgets[id].active = active;
    windows.replace(w, window_id);
}

void Widget::hide_widget()
{
    if (windows.get(window_id).active)
        Graphic::draw_rect(x, y, w, h, windows.get(window_id).bg);

    active = false;

    Window w = windows.get(window_id);
    w.widgets[id].active = active;
    windows.replace(w, window_id);
}

Widget create_widget(Window win)
{
    Widget w;
    w.window_id = win.id;
    w.id = win.widget_count;
    w.color = Graphic::rgb(255, 0, 0);

    w.x = win.avx;
    w.y = win.avy;

    win.avx += 10;

    if (width - win.avx <= 10)
    {
        win.avx = 10;
        win.avy += 30;
    }

    w.w = 30;
    w.h = 30;

    win.widgets[w.id] = w;
    win.widget_count++;

    windows.replace(win, win.id);

    return w;
}
