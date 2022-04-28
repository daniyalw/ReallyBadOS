#include "list.h"

namespace UI {

UI::List *list(UI::Window *win, int x, int y, int bg, int fg, int change) {
    UI::List *list = new UI::List();

    list->coords.x = x;
    list->coords.y = y;
    list->coords.w = list->width();
    list->coords.h = list->height();

    list->change = change;

    list->bg = bg;
    list->fg = fg;

    list->id = win->child_count;
    list->parent = win;

    win->childs[list->id] = list;
    win->child_count++;

    return list;
}

UI::List *list(UI::Window *win, int x, int y) {
    return list(win, x, y, Graphic::rgb(255, 255, 255), 0, Graphic::rgb(150, 150, 255));
}
}
