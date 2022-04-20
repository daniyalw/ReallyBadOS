#include "entry.h"

namespace UI {

Entry *entry(UI::Window *win, int x, int y, int bg, int fg) {
    Entry *entry = new Entry();

    memset(entry->text, 0, 100);
    memset(entry->name, 0, 20);
    strcpy(entry->name, "Entry");

    entry->bg = bg;
    entry->fg = fg;

    entry->coords.x = x;
    entry->coords.y = y;
    entry->coords.w = font_width * 20 + (entry->padding * 2);
    entry->coords.h = font_height + (entry->padding * 2);

    entry->id = win->child_count;
    entry->parent = win->id;

    win->childs[entry->id] = entry;
    win->child_count++;

    ui_objects[win->id] = win;

    return entry;
}

Entry *entry(UI::Window *win, int x, int y) {
    return entry(win, x, y, DEFAULT_BG, DEFAULT_FG);
}
}
