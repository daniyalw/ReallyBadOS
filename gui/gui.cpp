#include "gui.h"
#include "object.h"

inline void handle_mouse_click(coords_t coords, bool right, bool left, bool middle) {
    bool done = false;

    for (int z = 0; z < ui_obj_count; z++) {
        int id = z_order[z];

        UIObject *win = ui_objects[z];

        if (win->null)
            continue;

        for (int b = 0; b < win->child_count; b++) {
            UIObject *widget = win->childs[b];

            if ((widget->coords.x + win->coords.x) <= coords.x && (widget->coords.w + (widget->coords.x + win->coords.x)) >= coords.x) {
                if ((widget->coords.y + win->coords.y) <= coords.y && (widget->coords.h + (widget->coords.y + win->coords.y)) >= coords.y) {
                    if (z != 0) update_window_z(id, 0);

                    if (right || left || middle) {
                        for (int c = 0; c < ui_obj_count; c++) {
                            UIObject *w = ui_objects[c];

                            for (int i = 0; i < w->child_count; i++) {
                                UIObject *wi = w->childs[i];
                                wi->active = false;
                                w->childs[i] = wi;
                            }

                            ui_objects[w->id] = w;
                        }

                        widget->active = true;
                        win->childs[widget->id] = widget;
                        ui_objects[win->id] = win;
                    }

                    Event *event = create_event();
                    event->type = get_mouse_click_type(right, left, middle);

                    event->right = right;
                    event->left = left;
                    event->middle = middle;
                    event->coords = coords;

                    strcpy(event->name, "mouse");

                    widget->event_handle(widget, event);

                    done = true;

                    break;
                }
            }
        }

        if (done)
            break;
    }
}

inline void handle_key_entry(char key) {
    bool done = false;

    for (int z = 0; z < ui_obj_count; z++) {
        int id = z_order[z];

        auto win = ui_objects[id];

        if (win->null)
            continue;

        for (int b = 0; b < win->child_count; b++) {
            auto widget = win->childs[b];

            if (widget->active) {
                Event *event = create_event();
                strcpy(event->name, "keyboard");
                event->type = EVENT_KEY;
                event->key = key;

                widget->event_handle(widget, event);
                done = true;
            }
        }

        if (done)
            break;
    }
}

inline Event *create_event() {
    Event *event = new Event();

    memset(event->name, 0, 20);

    return event;
}

inline int get_mouse_click_type(bool right, bool left, bool middle) {
    if (right)
        return EVENT_MOUSE_RIGHT;
    else if (left)
        return EVENT_MOUSE_LEFT;
    else if (middle)
        return EVENT_MOUSE_MIDDLE;

    return EVENT_MOUSE_HOVER;
}
