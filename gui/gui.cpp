#include <gui/gui.h>
#include <gui/window.h>

void handle_mouse_click(coords_t coords, bool right, bool left, bool middle) {
    bool done = false;

    for (int z = 0; z < window_count; z++) {
        int id = windows_z[z];

        window_t win = windows[id];

        if (win.null)
            continue;

        for (int b = 0; b < win.widget_count; b++) {
            widget_t widget = win.widgets[b];

            if ((widget.coords.x + win.coords.x) <= coords.x && (widget.coords.w + (widget.coords.x + win.coords.x)) >= coords.x) {
                if ((widget.coords.y + win.coords.y) <= coords.y && (widget.coords.h + (widget.coords.y + win.coords.y)) >= coords.y) {
                    if (z > 0)
                        update_window_z(id, 0);

                    if (right || left || middle) {
                        for (int c = 0; c < window_count; c++) {
                            window_t w = windows[c];

                            for (int i = 0; i < w.widget_count; i++) {
                                log::warning("GUI: widget %d of window %d turned OFF", i, c);
                                widget_t wi = w.widgets[i];
                                wi.active = false;
                                w.widgets[i] = wi;
                            }

                            windows[c] = w;
                        }

                        log::info("GUI: widget %d of window %d turned ON", widget.id, win.id);

                        widget.active = true;
                        win.widgets[widget.id] = widget;
                        windows[win.id] = win;
                    }

                    done = true;
                    win_draw(win);
                    widget.mouse_click(id, widget, coords, right, left, middle);
                    break;
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

    for (int z = 0; z < window_count; z++) {
        int id = windows_z[z];

        window_t win = windows[id];

        if (win.null)
            continue;

        for (int b = 0; b < win.widget_count; b++) {
            widget_t widget = win.widgets[b];

            if (widget.active) {
                log::info("Widget found for key click: %d of window %d", widget.id, win.id);
                win_draw(win);
                widget.key_click(widget, key);
                done = true;
            }
        }

        if (done)
            break;
    }
}
