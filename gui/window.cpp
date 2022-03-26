#include <gui/window.h>
#include <gui/widget.h>
#include <gui/coords.h>
#include <utils.h>

window_t window_create(int x, int y, int bg, char *title, ...) {
    va_list va;
    coords_t coords;
    window_t window;

    va_start(va, title);
    char *out;
    out = vsprintf(out, title, va);
    va_end(va);

    coords.x = x;
    coords.y = y;
    coords.w = 200;
    coords.h = 200;

    window.bg = bg;

    window.coords = coords;
    strcpy(window.name, out);
    window.id = window_count;

    windows[window_count] = window;
    add_new_window(window);
    window_count++;

    return window;
}

void win_draw(window_t win) {
    // title
    Graphic::draw_rect(win.coords.x, win.coords.y - font_height - 2, win.coords.w, font_height + 2, Graphic::rgb(14, 0, 135));
    draw_string(win.name, win.coords.x, win.coords.y - font_height - 1, Graphic::rgb(255, 255, 255));

    // body
    Graphic::draw_rect(win.coords.x, win.coords.y, win.coords.w, win.coords.h, win.bg);

    for (int z = 0; z < win.widget_count; z++)
        if (win.widgets[z].to_draw)
            win.widgets[z].draw_widget(win.widgets[z], win.coords);
}

void add_new_window(window_t win) {
    add_window_at_location(0, win.id);
}

void remove_window_z(int _z) {
    for (int b = _z; b < window_count; b++)
        windows_z[b] = windows_z[b + 1];
}

void remove_window_id(int id) {
    int _z = -1;

    for (int z = 0; z < window_count; z++)
        if (windows_z[z] == id)
            _z = z;

    if (_z != -1) remove_window_z(_z);
}

void add_window_at_location(int _z, int id) {
    for (int b = window_count; b >= _z; b--)
        windows_z[b + 1] = windows_z[b];

    windows_z[_z] = id;
}

void update_window_z(int id, int new_z) {
    remove_window_id(id);
    add_window_at_location(new_z, id);
}
