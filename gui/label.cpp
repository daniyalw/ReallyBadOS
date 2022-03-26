#include <gui/label.h>
#include <gui/window.h>

void draw_label(widget_t label, coords_t coords) {
    int x = coords.x + label.coords.x;
    const int orig_x = x;
    int y = coords.y + label.coords.y;
    const int orig_y = y;
    int max_w = coords.x + coords.w;
    int max_h = coords.y + coords.h;
    char *text = (char *)label.data[0];
    int *wrap = (int *)label.data[1];

    int limit_x = max_w - x;
    int limit_y = max_h - y;

    if (x >= max_w || y >= max_h)
        if (wrap[0] != 1) return;

    draw_rect(x, y, limit_x, limit_y, label.bg);

    for (int z = 0; z < strlen(text); z++) {
        draw_char(text[z], x, y, label.fg);

        x += font_width;

        if (x >= max_w || y >= max_h) {
            if (wrap[0]) return;

            y += font_height + 1;
            x = orig_x;
        }
    }
}

// if wrap is 1, wrap is true
// else no wrap
label_t create_label(auto win, char *text, int x, int y, int wrap, int bg, int fg) {
    log::warning("Part A");

    label_t label;

    log::warning("Part B");

    label.id = win.widget_count;
    label.parent_id = win.id;
    label.data[0] = text;

    log::warning("Part C");

    int w[] = {wrap, 0};
    label.data[1] = w;

    log::warning("Part D");

    label.coords.x = x;
    label.coords.y = y;
    label.coords.w = strlen(text) * font_width;
    label.coords.h = font_height;

    log::warning("Part E");

    label.bg = bg;
    label.fg = fg;

    log::warning("Part F");

    label.mouse_click = default_mouse_click;
    label.key_click = default_key_click;
    label.draw_widget = draw_label;

    log::warning("Part G");

    return label;
}

label_t create_label(auto win, char *text, int x, int y, int wrap) {
    return create_label(win, text, x, y, wrap, DEFAULT_BG, DEFAULT_FG);
}
