#include "button.h"

UIButton *create_button(UIWindow *win, char *text, int x, int y, int bg, int fg, auto callback) {
    UIButton *btn = new UIButton();

    memset(btn->text, 0, 100);
    strcpy(btn->text, text);

    memset(btn->name, 0, 20);
    strcpy(btn->name, "UIButton");

    btn->bg = bg;
    btn->fg = fg;

    btn->coords.x = x;
    btn->coords.y = y;
    btn->coords.w = font_width * strlen(btn->text) + (btn->padding * 2);
    btn->coords.h = font_height + (btn->padding * 2);

    btn->id = win->child_count;
    btn->parent = win->id;

    btn->callback = callback;

    win->childs[btn->id] = btn;
    win->child_count++;

    ui_objects[win->id] = win;

    return btn;
}

UIButton *create_button(UIWindow *win, char *text, int x, int y, auto callback) {
    return create_button(win, text, x, y, DEFAULT_BG, DEFAULT_FG, callback);
}
