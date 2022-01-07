#pragma once

#include <gui/widget.h>
#include <gui/window.h>
#include <gui/event.h>

void handle_event_label(Widget w, gui_event_t * event)
{
    if (event->type == MOUSE_RIGHT_CLICK)
    {
        w.hide(w);
    }
}

void draw_label(Widget w)
{
    Window *win = windows[w.parent()];

    draw_string((char *)(w.extra[0]), w.getx() + win->x, w.gety() + win->y, (int)(w.extra[1]));
}

void hide_label(Widget w)
{
    Window *win = windows[w.parent()];

    draw_rect(w.getx() + win->x, w.gety() + win->y, std::len((char *)(w.extra[0])) * font_width, font_height, win->bg);
}

Widget create_label(char * text, int c)
{
    Widget w;

    w.setw(std::len(text) * font_width);
    w.seth(font_height);
    w.push_back_extra(text);
    w.push_back_extra((void *)c);
    w.set_draw(draw_label);
    w.set_hide(hide_label);
    w.set_event_handler(handle_event_label);

    return w;
}
