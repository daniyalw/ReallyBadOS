#pragma once

using namespace Graphic;

struct Widget
{
    int win_id;
    int id;
    int x, y;
    int padding;

    void (*draw)(Widget widget);
    void (*hide)(Widget widget);

    void set_draw(void (*func)(Widget widget))
    {
        draw = func;
    }

    void setx(int _x) { x = _x; }
    void sety(int _y) { y = _y; }
};

void test_draw(Widget widget)
{
    draw_rect(widget.x, widget.y, 10, 10, rgb(255, 0, 0));
}
