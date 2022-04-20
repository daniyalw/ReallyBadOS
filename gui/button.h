#pragma once

#include "window.h"
#include "object.h"

namespace UI {

class Button : public UI::Object {
public:
    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        callback(this, event);
    }

    virtual void draw(UI::Object *object, UI::Coords _coords) override {
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
                if (wrap)
                    break;

                y += font_height + 1;
                x = orig_x;
            }
        }
    }

    char text[100] = {NULL};
    int wrap = 0; // 0 - false; 1 - true
    int outer = 1; // outer is small little border; 1 - true; 0 - false
    int padding = 2;

    void (*callback)(Object*, Event*);
};

Button *button(Window *win, char *text, int x, int y, auto callback);
Button *button(UI::Window *win, char *text, int x, int y, int bg, int fg, auto callback);

}
