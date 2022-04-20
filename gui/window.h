#pragma once

#include "object.h"

namespace UI {

struct Window : public UI::Object {
    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        log::info("WINDOW EVENT %s\n", event->name);
    }

    virtual void draw(UI::Object *object, UI::Coords __coords) override {
        Graphic::draw_rect(object->coords.x, object->coords.y - font_height - 2, object->coords.w, font_height + 2, Graphic::rgb(14, 0, 135));
        draw_string(object->name, object->coords.x, object->coords.y - font_height - 1, Graphic::rgb(255, 255, 255));

        Graphic::draw_rect(object->coords.x, object->coords.y, object->coords.w, object->coords.h, object->bg);

        for (int z = 0; z < object->child_count; z++)
            if (object->childs[z]->to_draw)
                object->childs[z]->draw(object->childs[z], object->coords);
    }
};

Window *window(char *name, int bg, int fg, int font);

}

int find_z_from_id(int id);
void move_z_order_up(int pos, int size);
void add_z_order(int id);
void move_z_order_down(int pos, int size);
void remove_z(int id);

void remove_window_id(int id);
void remove_window(UI::Window *win);
void add_window(UI::Window *win);

void draw_window(UI::Window *win);

void update_window_z(int id, int pos);
void log_z_order();
