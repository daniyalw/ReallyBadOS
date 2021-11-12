#pragma once
#include "label.h"

Label::Label(Window win, char * t, int _padding, int _fg, int _bg)
{
    text = t;
    window = win;
    padding = _padding;
    x = win.getx();
    fg = _fg;
    bg = _bg;
    y = win.gety();
    w = len(t) * 9;
    h = 9;
}

void Label::draw()
{
    int _x=x, _y=y;

    if (window.is_active())
    {
        _x += padding;
        _y += padding;
        g_printf(text, _x, _y, fg, bg);
    }
}

void Label::hide()
{
    draw_rect(x+padding, y+padding, len(text)*9, 9, window.get_color());
}

void Label::move(int newx, int newy)
{
    this->hide();
    x = newx;
    y = newy;
    this->draw();
}
