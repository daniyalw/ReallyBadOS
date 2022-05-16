#include "checkbox.h"

namespace UI {

Checkbox *checkbox(UI::Window *win, int x, int y, int fg, auto callback) {
    Checkbox *chkbx = new Checkbox();

    memset(chkbx->name, 0, 20);
    strcpy(chkbx->name, "Checkbox");

    chkbx->fg = fg;
    chkbx->bg = DEFAULT_BG;

    chkbx->coords.x = x;
    chkbx->coords.y = y;
    chkbx->coords.w = chkbx->width();
    chkbx->coords.h = chkbx->height();

    chkbx->id = win->child_count;
    chkbx->parent = win;

    win->childs[chkbx->id] = chkbx;
    win->child_count++;

    chkbx->callback = callback;

    ui_objects[win->id] = win;

    return chkbx;
}

Checkbox *checkbox(UI::Window *win, int x, int y, auto callback) {
    return checkbox(win, x, y, Graphic::rgb(0, 0, 255), callback);
}

Checkbox *checkbox(UI::Window *win, int x, int y) {
    return checkbox(win, x, y, Graphic::rgb(0, 0, 255), default_callback);
}
}
