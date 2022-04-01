#include "gui.h"
#include "object.h"

void handle_mouse_click(coords_t coords, bool right, bool left, bool middle) {
    bool done = false;

    for (int z = 0; z < ui_obj_count; z++) {
        UIObject *win = ui_objects[z_order[z]];

        if (win->null)
            continue;

        for (int b = 0; b < win->child_count; b++) {
            UIObject *obj = win->childs[b];

            if ((obj->coords.x + win->coords.x) <= coords.x && (obj->coords.w + (obj->coords.x + win->coords.x)) >= coords.x) {
                if ((obj->coords.y + win->coords.y) <= coords.y && (obj->coords.h + (obj->coords.y + win->coords.y)) >= coords.y) {
                    update_window_z(win->id, 0);

                    if (right || left || middle) {
                        // make the active part of every widget for every window false
                        for (int c = 0; c < ui_obj_count; c++) {
                            auto w = ui_objects[c];

                            for (int i = 0; i < w->child_count; i++) {
                                auto wi = w->childs[i];
                                wi->active = false;
                                w->childs[i] = wi;
                            }

                            ui_objects[c] = w;
                        }

                        obj->active = true;
                        win->childs[obj->id] = obj;
                        ui_objects[win->id] = win;
                    }

                    Event event = create_event();
                    event.type = get_mouse_click_type(right, left, middle);

                    event.right = right;
                    event.left = left;
                    event.middle = middle;
                    event.coords = coords;

                    strcpy(event.name, "mouse");

                    obj->event_handle(obj, event);
                    done = true;
                }
            }
        }

        if (done)
            break;
    }
}

void handle_key_entry(char key) {
    log::info("GUI: entered key '%c'", key);
    bool done = false;

    for (int z = 0; z < ui_obj_count; z++) {
        int id = z_order[z];

        auto win = ui_objects[id];

        if (win->null)
            continue;

        for (int b = 0; b < win->child_count; b++) {
            auto widget = win->childs[b];

            if (widget->active) {
                Event event = create_event();
                strcpy(event.name, "keyboard");
                event.type = EVENT_KEY;
                event.key = key;

                widget->event_handle(widget, event);
                done = true;
            }
        }

        if (done)
            break;
    }
}

Event create_event() {
    Event event;

    event.coords.x = 0;
    event.coords.y = 0;
    event.coords.w = 0;
    event.coords.h = 0;

    memset(event.name, 0, 20);

    return event;
}

int get_mouse_click_type(bool right, bool left, bool middle) {
    if (right)
        return EVENT_MOUSE_RIGHT;
    else if (left)
        return EVENT_MOUSE_LEFT;
    else if (middle)
        return EVENT_MOUSE_MIDDLE;

    return EVENT_MOUSE_HOVER;
}
