#include "widget.h"

Widget::Widget(Window mywin, int height, int length)
{
    mywin.widgets[mywin.widget_i] = (int)this;
    mywin.widget_i++;
    window = mywin;
    l = length;
    h = height;
    _x = window._x + window.free_x;
    _y = window._y + window.free_y;
    mywin.update_free();
}

void Widget::draw()
{
    for (int z = _x; z < l+_x; z++)
    {
        for (int b = _y; b < h+_y; b++)
            putpixel(z, b, color);
    }
}

void Widget::hide()
{
    for (int z = _x; z < l+_x; z++)
    {
        for (int b = _y; b < h+_y; b++)
            putpixel(z, b, light_blue);
    }
}
