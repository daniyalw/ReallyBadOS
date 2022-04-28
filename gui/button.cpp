#include "button.h"

namespace UI {

Button *button(UI::Window *win, int x, int y, int bg, int fg, auto callback, char *text, va_list va) {
    Button *btn = new Button();

    memset(btn->text, 0, 100);
    vsprintf(btn->text, text, va);
    va_end(va);

    memset(btn->name, 0, 20);
    strcpy(btn->name, "Button");

    btn->bg = bg;
    btn->fg = fg;

    btn->coords.x = x;
    btn->coords.y = y;
    btn->coords.w = font_width * strlen(btn->text) + (btn->padding * 2);
    btn->coords.h = font_height + (btn->padding * 2);

    btn->id = win->child_count;
    btn->parent = win;

    btn->callback = callback;

    win->childs[btn->id] = btn;
    win->child_count++;

    ui_objects[win->id] = win;

    return btn;
}

Button *button(UI::Window *win, int x, int y, int bg, int fg, auto callback, char *text, ...) {
    va_list va;
    va_start(va, text);
    return button(win, x, y, bg, fg, callback, text, va);
}

Button *button(Window *win, int x, int y, auto callback, char *text, ...) {
    va_list va;
    va_start(va, text);
    return button(win, x, y, DEFAULT_BG, DEFAULT_FG, callback, text, va);
}

}
