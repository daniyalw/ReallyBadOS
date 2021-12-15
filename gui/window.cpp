#pragma once

#include <colors.h>
#include <font.h>

typedef struct {
    int x, y;
    int w, h;
    int parent_id;
    int id;
    int parent_z;
} widget_t;

typedef struct {
    int x, y;
    int w, h;
    int id;
    int z;
    widget_t * widgets[128];
    int widget_count;
    int avx, avy;
    char * title;
    bool active;
    bool dragging = false;
} window_t;

std::list<window_t *> windows;
int win_count = 0;

window_t * create_window(char * title)
{
    window_t * win;
    win->title = title;
    win->x = 40;
    win->y = 40;
    win->active = false;
    win->w = 45 + (std::len(title) * (font_double_on ? 17 : 10));
    win->h = 60;
    win->avx = 5;
    win->avy = 5;
    win->z = 0;
    win->id = win_count;
    win_count++;


    windows.insert_at(win, win->z);

    return win;
}

widget_t * create_widget(window_t * win)
{
    widget_t * widget;

    widget->parent_id = win->id;
    widget->parent_z = win->z;
    widget->x = win->x;
    widget->y = win->x;
    widget->w = 10;
    widget->h = 5;
    widget->id = win->widget_count;

    win->widget_count++;
    win->widgets[widget->id] = widget;

    windows.replace(win, win->id);

    return widget;
}

void draw_widget(widget_t * widget)
{
    window_t * win = windows.get(widget->parent_z);

    if (win->active)
        Graphic::draw_rect(widget->x, widget->y, widget->w, widget->h, Graphic::rgb(255, 0, 0));
}

void draw_window(window_t * win)
{
    win->active = true;

    windows.replace(win, win->id);

    Graphic::draw_rect(win->x, win->y, win->w, win->h, Graphic::rgb(0, 255, 0));

    Graphic::draw_rect(win->x, win->y - (font_double_on ? 18 : 11), win->w, (font_double_on ? 18 : 11), Graphic::rgb(0, 0, 255));

    g_printf(win->title, win->x, win->y - ((font_double_on ? 17 : 10)));

    for (int z = 0; z < win->widget_count; z++)
    {
        draw_widget(win->widgets[z]);
    }
}

void move_window(window_t * win)
{
    Graphic::draw_rect(win->x, win->y, win->w, win->h, Graphic::rgb(0, 0, 0)); //hide
    Graphic::draw_rect(win->x, win->y, win->w, win->h, Graphic::rgb(0, 255, 0)); //draw
}

window_t * get_window_pixel(int x, int y)
{
    int win_pos;
    window_t * win;

    for (int z = 0; z < windows.size(); z++)
    {
        win = windows.get(z);

        for (int _x = win->x; _x < win->x + win->w; _x++)
        {
            for (int _y = win->y; _y < win->y + win->h; _y++)
            {
                if ((_x + _y * width) == (x + y * width))
                {
                    return win;
                }
            }
        }
    }

    return NULL;
}
