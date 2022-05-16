#include "gui.h"
#include "object.h"

void handle_mouse_click(UI::Coords coords, bool right, bool left, bool middle) {
    bool done = false;

    if (!right && !left && !middle) {
        for (int z = 0; z < ui_obj_count; z++) {
            UI::Window *obj = (UI::Window *)ui_objects[z];
            obj->dragging = false;
        }
        return;
    }

    for (int z = 0; z < ui_obj_count; z++) {
        int id = z_order[z];

        UI::Window *win = (UI::Window *)ui_objects[id];

        if (!win)
            continue;

        if (!win->to_draw) {
            continue;
        }

        if (win->coords.x > coords.x) {
            // this window can't be it; the widget is outside the window
            continue;
        }

        if ((win->coords.x + win->coords.w) < coords.x) {
            return;
        }

        if (win->coords.y >= coords.y && (win->coords.y - TITLEBAR_HEIGHT) <= coords.y) {
            if (!left) {
                return;
            }
            // titlebar clicked
            auto c = win->coords;
            log::info("titlebar clicked: window coords: x: %d y: %d w: %d h: %d", c.x, c.y, c.w, c.h);
            if (coords.x >= (c.x + c.w - (CLOSE_WIN_X)) && coords.x <= (c.x + c.w)) {
                // TODO close window
                log::info("Close window clicked\n");
                remove_window_id(win->id);
                Graphic::redraw_background_picture(array);
                draw_all_windows();
            } else if (coords.x >= (c.x + c.w - (HIDE_WIN_X)) && coords.x <= (c.x + c.w - (CLOSE_WIN_X))) {
                log::info("Hide window clicked\n");
                remove_window_id(win->id); // TODO make sure window can come back onto z order list
                win->hide();
                Graphic::redraw_background_picture(array);
                draw_all_windows();
            } else if (coords.x >= c.x && coords.x <= (c.x + c.w - (HIDE_WIN_X))) {
                if (win->drag()) {
                    // TODO actually drag window
                    Graphic::redraw_background_picture(array);
                    draw_all_windows();
                } else {
                    win->drag(true);
                }
            }
        } else if (win->coords.y > coords.y || (win->coords.y + win->coords.h) < coords.y) {
            // window outside of range
            continue;
        }

        log::info("Hit window %d (%s)", id, win->name);

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

                                w->childs[i] = wi;
                                ui_objects[c] = w;
                            }
                        }

                        widget->active = true;
                        win->childs[b] = widget;
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

void handle_key_entry(char key) {
    bool done = false;

    for (int z = 0; z < ui_obj_count; z++) {
        int id = z_order[z];

        UI::Window *win = (UI::Window *)ui_objects[z];

        if (!win) {
            continue;
        }

        for (int b = 0; b < win->child_count; b++) {
            auto widget = win->childs[b];

            if (strcmp(widget->name, "Entry") == 0) {
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

void default_callback(UI::Object *obj, UI::Event *event) {
}
