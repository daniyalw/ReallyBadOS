#pragma once

#include "gui.h"
#include <colors.h>

Window::Window()
{
    id = windows.size();
    avx = 10;
    w = 60;
    h = 60;
    avy = 10;
    z = 0;
    windows.insert_at(this, this->z);
}

int Window::get_id()
{
    return id;
}

void Window::draw_window()
{
    for (int z = 0; z < w; z++)
        for (int b = 0; b < h; b++)
            Graphic::SetPixel(z, b, bg);

    active = true;

    Window * w = windows.get(id);
    w->active = active;
    windows.replace(w, id);

    for (int z = 0; z < widget_count; z++)
    {
        if (this->widgets[z]->active)
            widgets[z]->draw_widget();
    }
}

Widget::Widget()
{
}
