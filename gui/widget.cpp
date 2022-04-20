#include "widget.h"

void draw_widget(UI::Object *obj) {
    obj->to_draw = true;
}

void hide_widget(UI::Object *obj) {
    obj->to_draw = false;
}
