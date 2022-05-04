#pragma once

#include "window.h"
#include "object.h"

namespace UI {

class Entry : public UI::Object {
public:
    void draw_text() {
        int x = coords.x + parent->coords.x + padding;
        int y = coords.y + parent->coords.y + padding;

        const int orig_x = x;
        const int orig_y = y;

        const int max_x = x + coords.w;
        const int max_y = y + coords.h;

        for (int z = 0; z < offset; z++) {
            draw_char(text[z], x, y, fg);
            x += font_width;
        }
    }

    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        if (event->type == EVENT_KEY) {
            text[offset] = event->key;
            offset++;

            draw_text();
        }
    }

    virtual void draw_object(UI::Object *object, UI::Coords _coords) override {
        if (object->bg != BG_TRANSPARENT) {
            Graphic::draw_rect(object->coords.x + _coords.x, object->coords.y + _coords.y, object->coords.w + padding * 2, object->coords.h + padding * 2, object->bg);
        }

        Graphic::draw_empty_rect(object->coords.x + _coords.x, object->coords.y + _coords.y, object->coords.w, object->coords.h, object->fg);

        draw_text();
    }

    virtual int height() {
        return font_height + padding + 2;
    }

    virtual int width() {
        return font_width * 20 + padding + 2;
    }

    char *get() {
        return strdup(text);
    }

    int offset = 0;
    int wrap = 0; // 0 - false; 1 - true
    int outer = 1; // outer is small little border; 1 - true; 0 - false
    int padding = 2;
};

Entry *entry(UI::Window *win, int x, int y, int bg, int fg);
Entry *entry(UI::Window *win, int x, int y);

}
