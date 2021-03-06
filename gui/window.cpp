#include <gui/coords.h>
#include <utils.h>
#include "window.h"

namespace UI {

Window *window(char *name, int bg, int fg, int x, int y) {
    Window *win = new Window();

    memset(win->name, 0, 20);
    strcpy(win->name, name);

    win->bg = bg;
    win->fg = fg;

    win->coords.x = x;
    win->coords.y = y;
    win->coords.w = 500;
    win->coords.h = 300;

    win->parent = NULL;

    add_window(win);

    return win;
}

Window *window(char *name, int x, int y) {
    return window(name, DEFAULT_BG, DEFAULT_FG, x, y);
}

Window *window(char *name) {
    return window(name, DEFAULT_BG, DEFAULT_FG, 400, 400);
}

}

int find_z_from_id(int id) {
    for (int z = 0; z < ui_obj_count; z++)
        if (z_order[z] == id)
            return z;

    return -1;
}

void move_z_order_up(int pos) {
    for (int z = ui_obj_count - 1; z >= pos; z--) {
        z_order[z + 1] = z_order[z];
    }
}

void add_z_order(int id) {
    move_z_order_up(0);
    z_order[0] = id;
}

void move_z_order_down(int pos) {
    for (int z = pos; z < ui_obj_count; z++) {
        z_order[z] = z_order[z + 1];
    }
}

void remove_window_id(int id) {
    int z = find_z_from_id(id);

    if (z != -1) {
        move_z_order_down(z);
        ui_obj_count--;
    } else {
        log::warn("GUI: invalid z found for id %d", id);
    }
}

int *get_z_order() {
    return z_order;
}

void remove_window(UI::Window *win) {
    remove_window_id(win->id);
}

void add_window(UI::Window *win) {
    win->id = ui_obj_count;
    ui_objects[win->id] = win;
    ui_obj_count++;

    add_z_order(win->id);
}

void draw_window(UI::Window *win) {
    win->draw_object(win, win->coords);
}

void update_window_z(int id, int pos) {
    remove_window_id(id);
    move_z_order_up(pos);
    z_order[pos] = id;
    ui_obj_count++;
}

void draw_all_windows() {
    for (int z = ui_obj_count - 1; z >= 0; z--) {
        UI::Window *win = (UI::Window *)ui_objects[z_order[z]];

        if (win->to_draw) {
            draw_window(win);
        }
    }
}

// debug
void log_z_order() {
    for (int z = 0; z < ui_obj_count; z++) {
        log::warning("ID %d at z %d", z_order[z], z);
    }
}
