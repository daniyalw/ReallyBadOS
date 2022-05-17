#pragma once

#include "window.h"
#include "object.h"

namespace UI {

class Label : public UI::Object {
public:
    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        log::error("GUI: label: received event of type %s\n", event->name);
    }

    virtual void draw_object(UI::Object *object, UI::Coords _coords) override {
        if (object->bg != BG_TRANSPARENT) {
            Graphic::draw_rect(object->coords.x + _coords.x, object->coords.y + _coords.y, object->coords.w, object->coords.h, object->bg);
        }

        const int max_w = _coords.x + object->coords.x + object->coords.w;
        const int max_h = _coords.y + object->coords.y + object->coords.h;

        int x = object->coords.x + _coords.x + padding;
        int y = object->coords.y + _coords.y + padding;
        const int orig_x = x;

        for (int z = 0; z < strlen(text); z++) {
            draw_char(text[z], x, y, object->fg);

            x += font_width;
        }
    }

    virtual int height() override {
        return padding * 2 + font_height;
    }

    virtual int width() override {
        return strlen(text) * font_width + padding * 2;
    }

    int wrap = 0; // 0 - false; 1 - true
    int padding = 0;
};

Label *label(UI::Window *win, int x, int y, int bg, int fg, char *text, va_list va);
Label *label(UI::Window *win, int x, int y, int bg, int fg, char *text, ...);
Label *label(UI::Window *win, int x, int y, char *text, ...);

}
