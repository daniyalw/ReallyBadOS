#pragma once

struct Event {
    char name[20];
    int type;

    bool right = false, middle = false, left = false;
    char key = NULL;

    coords_t coords;
};

struct UIObject {
    char name[20];

    // colors
    int bg, fg;
    int fsize; // font size

    // dimensions
    coords_t coords;

    UIObject *childs[10]; // children
    int child_count = 0;

    bool dragged = false, null = false, active = false, to_draw = false;
    int id, parent;

    virtual void event_handle(UIObject*, Event*) {}
    virtual void draw(UIObject*, coords_t) {}
};

UIObject *ui_objects[100];
int ui_obj_count = 0;

int z_order[100];
