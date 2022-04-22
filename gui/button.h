#pragma once

#include "window.h"
#include "object.h"

namespace UI {

class Button : public UI::Object {
public:
    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        callback(this, event);
    }

    virtual void draw_object(UI::Object *object, UI::Coords _coords) override {
        if (object->bg != BG_TRANSPARENT) {
            Graphic::draw_rect(object->coords.x + _coords.x, object->coords.y + _coords.y, object->coords.w, object->coords.h, object->bg);
        }

        Graphic::draw_empty_rect(object->coords.x + _coords.x, object->coords.y + _coords.y, object->coords.w, object->coords.h, object->fg);

        const int max_w = _coords.x + object->coords.x + object->coords.w;
        const int max_h = _coords.y + object->coords.y + object->coords.h;

        int x = object->coords.x + _coords.x + padding;
        int y = object->coords.y + _coords.y + padding;
        const int orig_x = x;

        for (int z = 0; z < strlen(text); z++) {
            draw_char(text[z], x, y, object->fg);

            x += font_width;

            if (x >= max_w || y >= max_h) {
                break;
            }
        }
    }

    virtual int height() {
        return font_height + padding + 2;
    }

    virtual int width() {
        return padding + 2 + strlen(text) * font_height;
    }

    int wrap = 0; // 0 - false; 1 - true
    int outer = 1; // outer is small little border; 1 - true; 0 - false
    int padding = 10;

    void (*callback)(Object*, Event*);
};

Button *button(UI::Window *win, int x, int y, int bg, int fg, auto callback, char *text, va_list va);
Button *button(UI::Window *win, int x, int y, int bg, int fg, auto callback, char *text, ...);
Button *button(Window *win, int x, int y, auto callback, char *text, ...);

}
