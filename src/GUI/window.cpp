#include "window.h"

Window::Window(int __x, int __y)
{
    _x = __x;
    _y = __y;
}

void Window::draw()
{

	for (int z = _x; z < wh+_y; z++)
	{
		for (int b = _y; b < ww+_x; b++)
			putpixel(z, b, color);
	}

	for (int z = _x; z < ww+_x; z++)
	{
		putpixel(z, _y-1, top);
        putpixel(z, _y-2, top);
	}
}

void Window::hide()
{

	for (int z = _x; z < wh+_y; z++)
	{
		for (int b = _y; b < ww+_x; b++)
			putpixel(z, b, light_blue);
	}

	for (int z = _x; z < ww+_x; z++)
	{
		putpixel(z, _y-1, light_blue);
        putpixel(z, _y-2, light_blue);
	}
}

void Window::move(int __x, int __y)
{
    hide();
    _x = __x;
    _y = __y;
    draw();
}

void Window::resize(int h, int w)
{
    hide();
    ww = w;
    wh = h;
    draw();
}

void Window::update_free()
{
    free_x += 2;
    free_y += 2;
}
