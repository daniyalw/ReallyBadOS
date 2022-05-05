#pragma once

namespace UI {

struct Event {
    char name[20];
    int type;

    bool right = false, middle = false, left = false;
    char key = NULL;

    UI::Coords coords;
};

struct Object {
    char name[20];

    // colors
    int bg, fg, replace_c;
    int fsize; // font size

    // dimensions
    UI::Coords coords;

    Object *childs[10]; // children
    int child_count = 0;

    char text[100];

    bool dragged = false, null = false, active = false, to_draw = false;
    int id;

    Object *parent;

    virtual void event_handle(Object*, Event*) {
        log::warn("Default event handler called");
    }

    virtual void draw_object(Object*, Coords) {
        log::warn("Default draw object called");
    }

    virtual int height() { return 0; }
    virtual int width() { return 0; }

    void draw() {
        to_draw = true;
    }

    void hide() {
        to_draw = false;
    }
};

}

UI::Object *ui_objects[100];
int ui_obj_count = 0;

int z_order[100];
