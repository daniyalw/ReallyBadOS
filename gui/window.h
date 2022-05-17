#pragma once

#include "object.h"
#include "gui.h"

namespace UI {

struct Window : public UI::Object {
    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        log::info("GUI: window event %s\n", event->name);
    }

    virtual void draw_object(UI::Object *object, UI::Coords __coords) override {
        int _bg = TITLEBAR_INACTIVE_BG;
        int _fg = TITLEBAR_INACTIVE_FG;

        if (z_order[0] == id) {
            _bg = TITLEBAR_ACTIVE_BG;
            _fg = TITLEBAR_ACTIVE_FG;
        }

        Graphic::draw_rect(coords.x, coords.y - (TITLEBAR_HEIGHT), coords.w, TITLEBAR_HEIGHT + 2, _bg);
        Graphic::draw_rect(coords.x + coords.w - (HIDE_WIN_X), coords.y - (HIDE_WIN_Y), HIDE_WIN_X, HIDE_WIN_Y, TITLEBAR_HIDE_COLOR);
        Graphic::draw_rect(coords.x + coords.w - (CLOSE_WIN_X), coords.y - (TITLEBAR_HEIGHT), CLOSE_WIN_X, CLOSE_WIN_X, TITLEBAR_CLOSE_COLOR);
        draw_string(name, coords.x + 3, coords.y - (TITLEBAR_HEIGHT) + 3, _fg);

        Graphic::draw_rect(coords.x, coords.y, coords.w, coords.h, bg);

        for (int z = 0; z < object->child_count; z++)
            if (object->childs[z]->to_draw)
                object->childs[z]->draw_object(object->childs[z], object->coords);
    }

    bool drag() {
        return dragging;
    }

    void drag(int _drag) {
        dragging = _drag;
    }

    bool dragging = false;
};

Window *window(char *name, int bg, int fg, int x, int y);
Window *window(char *name, int x, int y);
Window *window(char *name);

}

int find_z_from_id(int id);
void move_z_order_up(int pos);
void add_z_order(int id);
void move_z_order_down(int pos);

void remove_window_id(int id);
void remove_window(UI::Window *win);
void add_window(UI::Window *win);

void draw_window(UI::Window *win);

void update_window_z(int id, int pos);
void log_z_order();

void draw_all_windows();
int *get_z_order();
