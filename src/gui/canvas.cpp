#pragma once
#include "canvas.h"

Canvas::Canvas(Window win)
{
    window = win;
    x = win.getx();
    y = win.gety();
    w = win.getw();
    h = win.geth();
}

void Canvas::draw()
{
    if (window.is_active())
        draw_rect(x, y, w, h, rgb(0, 255, 0));
}

void Canvas::hide()
{
    if (window.is_active())
    {
        draw_rect(x, y, w, h, window.get_color());
    }
}

void Canvas::move_to(int newx, int newy)
{
    window.hide();
    x = newx;
    y = newy;
    window.move(x, y);
    this->draw();
}

void Canvas::putpixel(int _x, int _y, int _color)
{
    if (window.is_active()) {
        _x += x;
        _y += y;
        SetPixel(_x, _y, _color);
    }
}
