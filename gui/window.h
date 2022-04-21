#pragma once

#include "object.h"

namespace UI {

struct Window : public UI::Object {
    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        log::info("WINDOW EVENT %s\n", event->name);
    }

    virtual void draw_object(UI::Object *object, UI::Coords __coords) override {
        Graphic::draw_rect(coords.x, coords.y - TITLEBAR_HEIGHT, coords.w, TITLEBAR_HEIGHT, Graphic::rgb(0, 0, 255));
        log::info("Coords: %d, %d, %d, %d, %d", coords.x + coords.w - CLOSE_WIN_X, coords.y - TITLEBAR_HEIGHT, CLOSE_WIN_X, CLOSE_WIN_X, coords.x + coords.w);
        Graphic::draw_rect(coords.x + coords.w - CLOSE_WIN_X, coords.y - TITLEBAR_HEIGHT, CLOSE_WIN_X, CLOSE_WIN_X, Graphic::rgb(255, 0, 0));
        draw_string(name, coords.x, coords.y - TITLEBAR_HEIGHT + 1, Graphic::rgb(255, 255, 255));

        Graphic::draw_rect(coords.x, coords.y, coords.w, coords.h, bg);

        for (int z = 0; z < object->child_count; z++)
            if (object->childs[z]->to_draw)
                object->childs[z]->draw_object(object->childs[z], object->coords);
    }
};

Window *window(char *name, int bg, int fg, int font);
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
