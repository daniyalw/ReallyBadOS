#pragma once

using namespace Graphic;

#include <gui/window.h>
#include <gui/widget.h>
#include <gui/gui.h>

Window::Window()
{
    id = windows.size();
    windows.push_back(this);
}

void Window::draw()
{
    draw_rect(x, y - font_height - TITLE_PADDING * 2, w, font_height + TITLE_PADDING * 2, 0);
    draw_rect(x + w - CLOSE_B, y - font_height - TITLE_PADDING * 2, CLOSE_B, font_height + TITLE_PADDING * 2, red);

    draw_string(title, x, y - font_height - TITLE_PADDING, white);

    draw_rect(x, y, w, h, bg);

    for (int z = 0; z < widget_count; z++)
    {
        widgets[z].draw(widgets[z]);
    }
}

void Window::set(int _x, int _y, int _w, int _h)
{
    x = _x;
    y = _y;
    w = _w;
    h = _h;

    windows.replace(this, this->id);
}

void Window::add_widget(Widget w)
{
    int __copy = widget_count;
    int __id_copy = id;

    w.set_id(__copy, __id_copy);

    widgets[widget_count] = w;
    widget_count++;

    windows.replace(this, id);
}

void Window::replace_widget(Widget w, int _id)
{
    widgets[_id] = w;
    windows.replace(this, id);
}

void clicked_on_widget(int x, int y, int type)
{
    for (int z = 0; z < windows.size(); z++)
    {
        Window * win = windows[z];

        for (int b = 0; b < win->widget_count; b++)
        {
            Widget widget = win->widgets[b];

            if (x >= widget.getx() && x <= (widget.getw() + widget.getx()))
            {
                if (y >= widget.gety() && y <= (widget.gety() + widget.geth()))
                {
                    Kernel::system_log("Widget found!\n");

                    gui_event_t * event;

                    event->type = type;
                    event->mx = x;
                    event->my = y;

                    widget.handle_event(widget, event);
                }
            }
        }
    }

    Kernel::system_log("No widget clicked\n");
}
