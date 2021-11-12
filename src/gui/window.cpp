#pragma once
#include "window.h"

Window::Window()
{
    shown = false;
    w = 100;
    h = 100;
    x = 100;
    y = 100;
    z = 0;
    id = win_count;
    windows[win_count] = this;
    win_count++;
}

int Window::get_color()
{
    return color;
}

bool Window::is_active()
{
    return shown;
}

int Window::getx()
{
    return x;
}

int Window::gety()
{
    return y;
}

int Window::getw()
{
    return w;
}

int Window::geth()
{
    return h;
}

void Window::draw()
{
    shown = true;
    draw_rect(x, y, w, h, color);
    draw_rect(x, y-5, w, 5, 0x345);
    draw_empty_rect(x-1, y-6, w+1, h+6, 0);

    save_window();
}

void Window::hide()
{
    shown = false;
    draw_rect(x, y, w, h, 0x9999);
    draw_rect(x, y-5, w, 5, 0x9999);
    draw_empty_rect(x-1, y-6, w+2, h+7, 0x9999);

    save_window();
}

void Window::move(int newx, int newy)
{
    this->hide();
    x = newx;
    y = newy;
    this->draw();

    save_window();
}

void Window::putpixel(int _x, int _y, int _color)
{
    if (this->is_active()) {
        _x += x;
        _y += y;
        SetPixel(_x, _y, _color);
    }

    save_window();
}

void Window::save_window()
{
    windows[id] = this;
}

void init_gui()
{
    for (int z = 0; z < 128; z++)
        windows[z] = 0;
}
