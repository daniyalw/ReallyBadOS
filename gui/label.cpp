#include "label.h"
#include "window.h"

namespace UI {
    
Label *label(UI::Window *win, char *text, int x, int y, int bg, int fg) {
    Label *label = new Label();

    memset(label->text, 0, 100);
    strcpy(label->text, text);

    memset(label->name, 0, 20);
    strcpy(label->name, "Label");

    label->bg = bg;
    label->fg = fg;

    label->coords.x = x;
    label->coords.y = y;
    label->coords.w = font_width * strlen(label->text);
    label->coords.h = font_height;

    label->id = win->child_count;
    label->parent = win->id;

    win->childs[label->id] = label;
    win->child_count++;

    return label;
}

Label *label(UI::Window *win, char *text, int x, int y) {
    return label(win, text, x, y, DEFAULT_BG, DEFAULT_FG);
}
}
