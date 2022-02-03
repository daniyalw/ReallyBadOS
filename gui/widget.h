#pragma once

using namespace Graphic;

struct Widget
{
    int win_id;
    int id;
    int x, y, bg, fg;
    int w, h;
    int padding;

    void **extras;

    void (*draw)(Widget widget);
    void (*hide)(Widget widget);

    void set_draw(void (*func)(Widget widget))
    {
        draw = func;
    }

    void setx(int _x) { x = _x; }
    void sety(int _y) { y = _y; }

    void setw(int _w) { w = _w; }
    void seth(int _h) { h = _h; }

    void setbg(int _bg) { bg = _bg; }
    void setfg(int _fg) { fg = _fg; }

    void set_extra(int index, void *data) { extras[index] = data; }
    void set_padding(int _padding) { padding = _padding; }

    void set_id(int _id) { id = _id; }
    void set_winid(int _win) { win_id = _win; }
};
