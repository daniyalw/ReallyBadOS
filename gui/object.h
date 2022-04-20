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
    int bg, fg;
    int fsize; // font size

    // dimensions
    UI::Coords coords;

    Object *childs[10]; // children
    int child_count = 0;

    bool dragged = false, null = false, active = false, to_draw = false;
    int id, parent;

    virtual void event_handle(Object*, Event*) {}
    virtual void draw(Object*, Coords) {}
};

}

UI::Object *ui_objects[100];
int ui_obj_count = 0;

int z_order[100];
