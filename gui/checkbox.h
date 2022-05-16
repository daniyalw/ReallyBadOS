#pragma once

namespace UI {

class Checkbox : public UI::Object {
public:
    void draw_helper() {
        auto _coords = parent->coords;
        auto x = coords.x + _coords.x;
        auto y = coords.y + _coords.y;

        if (checked) {
            Graphic::draw_rect(x, y, total, total, fg);
        } else {
            Graphic::draw_rect(x, y, total, total, bg);
            Graphic::draw_empty_rect(x, y, total, total, fg);
        }
    }

    virtual void event_handle(UI::Object *object, UI::Event *event) override {
        if (event->type == EVENT_MOUSE_LEFT) {
            if (checked) {
                checked = 0;
            } else {
                checked = 1;
            }

            draw_helper();
        }

        callback(object, event);
    }

    virtual void draw_object(UI::Object *object, UI::Coords _coords) override {
        draw_helper();
    }

    virtual int height() {
        return total;
    }

    virtual int width() {
        return total;
    }

    bool is() {
        return checked;
    }

    bool checked = 0;
    int space = 10;
    int total = 12;

    void (*callback)(UI::Object*, UI::Event*);
};

Checkbox *checkbox(UI::Window *win, int x, int y, int fg, auto callback);
Checkbox *checkbox(UI::Window *win, int x, int y, auto callback);
Checkbox *checkbox(UI::Window *win, int x, int y);

}
