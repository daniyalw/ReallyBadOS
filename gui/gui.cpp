#include "gui.h"
#include "object.h"

void handle_mouse_click(UI::Coords coords, bool right, bool left, bool middle) {
    bool done = false;

    if (!right && !left && !middle) {
        return;
    }

    for (int z = 0; z < ui_obj_count; z++) {
        int id = z_order[z];

        UI::Object *win = ui_objects[z];

        if (win->null)
            continue;

        for (int b = win->child_count - 1; b >= 0; b--) /* top to bottom */ {
            UI::Object *widget = win->childs[b];

            if (!widget->to_draw) {
                continue; // the widget was not drawn on-screen
            }

            if ((widget->coords.x + win->coords.x) <= coords.x && (widget->coords.w + (widget->coords.x + win->coords.x)) >= coords.x) {
                if ((widget->coords.y + win->coords.y) <= coords.y && (widget->coords.h + (widget->coords.y + win->coords.y)) >= coords.y) {
                    if (right || left || middle) {
                        if (z_order[z] != id) update_window_z(id, 0);

                        for (int c = 0; c < ui_obj_count; c++) {
                            UI::Object *w = ui_objects[c];

                            for (int i = 0; i < w->child_count; i++) {
                                UI::Object *wi = w->childs[i];
                                wi->active = false;
                            }
                        }

                        widget->active = true;
                    }

                    UI::Event *event = create_event();
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

    for (int c = 0; c < ui_obj_count; c++) {
        UI::Object *w = ui_objects[c];

        for (int i = 0; i < w->child_count; i++) {
            UI::Object *wi = w->childs[i];
            wi->active = false;
        }
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
                UI::Event *event = create_event();
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

inline UI::Event *create_event() {
    UI::Event *event = new UI::Event();

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
