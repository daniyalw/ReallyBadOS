#pragma once

#include "window.h"
#include "object.h"

struct UILabel : public UIObject {
    virtual void event_handle(UIObject *object, Event *event) override {
        log::error("Label: received event of type %s\n", event->name);
    }

    virtual void draw(UIObject *object, coords_t _coords) override {
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
    int padding = 0;
};

UILabel *create_label(UIWindow *win, char *text, int x, int y, int bg, int fg);
UILabel *create_label(UIWindow *win, char *text, int x, int y);
