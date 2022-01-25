#pragma once

#include <gui/event.h>

class Window;
class Widget;

class Widget
{
    friend class Window;

    int parent_id;
    int id;

    int x, y, w, h;

public:
    std::list<void *> extra;

    void set(int _x, int _y, int _w, int _h)
    {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }

    int getx() { return x; }
    int gety() { return y; }
    int getw() { return w; }
    int geth() { return h; }
    int parent() { return parent_id; }
    void setx(int _x) { x = _x; }
    void sety(int _y) { y = _y; }
    void setw(int _w) { w = _w; }
    void seth(int _h) { h = _h; }

    void (*draw)(Widget);
    void (*hide)(Widget);
    void (*handle_event)(Widget, gui_event_t *);

    void set_draw(void (*func)(Widget))
    {
        draw = func;
    }

    void set_hide(void (*func)(Widget))
    {
        hide = func;
    }

    void set_event_handler(void (*func)(Widget, gui_event_t *))
    {
        handle_event = func;
    }

    void push_back_extra(void *b) { extra.push_back(b); }

protected:

    void set_id(int _id, int _parent_id)
    {
        id = id;
        parent_id = _parent_id;
    }
};
