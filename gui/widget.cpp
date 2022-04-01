#include "widget.h"

void draw_widget(UIObject *obj) {
    obj->to_draw = true;
}

void hide_widget(UIObject *obj) {
    obj->to_draw = false;
}
