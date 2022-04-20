#pragma once

#include "window.h"
#include "object.h"

namespace UI {

class Entry : public UI::Object {
public:
    void draw_text() {
        int x = coords.x + ui_objects[parent]->coords.x;
        int y = coords.y + ui_objects[parent]->coords.y;

        const int orig_x = x;
        const int orig_y = y;

        const int max_w = x + coords.w;
        const int max_h = y + coords.h;

        if (strlen(text) <= 20) {
            for (int z = 0; z < strlen(text); z++) {
                draw_char(text[z], x, y, fg);

                x += font_width;

                if (x >= max_w || y >= max_h) {
                    if (wrap)
                        break;

                    y += font_height + 1;
                    x = orig_x;
                }
            }
        } else {
            int length = strlen(text);

            for (int z = length - 20; z < length; z++) {
                draw_char(text[z], x, y, fg);

                x += font_width;

                if (x >= max_w || y >= max_h) {
                    if (wrap)
                        break;

                    y += font_height + 1;
                    x = orig_x;
                }
            }
        }
    }

    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        if (event->type == EVENT_KEY) {
            text[offset] = event->key;
            offset++;

            draw_text();
        }
    }

    virtual void draw(UI::Object *object, UI::Coords _coords) override {
        if (object->bg != BG_TRANSPARENT) {
            Graphic::draw_rect(object->coords.x + _coords.x, object->coords.y + _coords.y, object->coords.w, object->coords.h, object->bg);
        }

        Graphic::draw_empty_rect(object->coords.x + _coords.x, object->coords.y + _coords.y, object->coords.w, object->coords.h, object->fg);

        draw_text();
    }

    char text[20] = {NULL};
    int offset = 0;
    int wrap = 0; // 0 - false; 1 - true
    int outer = 1; // outer is small little border; 1 - true; 0 - false
    int padding = 2;
};

Entry *entry(UI::Window *win, int x, int y, int bg, int fg);
Entry *entry(UI::Window *win, int x, int y);

}
