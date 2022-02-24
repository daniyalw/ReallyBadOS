#include <gui/entry.h>

void handle_entry_key(entry_t entry, char key)
{
    char *keys = (char *)entry.data[0];
    int *count = (int *)entry.extra[0];

    keys = get(keys, "%s%c\0", keys, key);
    int c = count[0];
    c += 1;
    log::info("Count 0: %d", c);
    count[0] = c;

    entry.extra[0] = count;
    entry.data[0] = keys;

    int vc = ((int *)entry.extra[0])[0];
    log::info("Count 1: %d", vc);

    window_t win = windows[entry.parent_id];
    win.widgets[entry.id] = entry;
    windows[win.id] = win;

    log::info("Count 2: %d", vc);

    entry_t a = windows[entry.parent_id].widgets[entry.id];
    log::info("Count: %d", ((int *)a.extra[0])[0]);

    int beg_x = count[0] * font_width;
    beg_x += entry.coords.x;
    beg_x += win.coords.x;

    int beg_y = entry.coords.y + win.coords.y;

    for (int z = 0; z < font_height; z++)
        for (int b = 0; b < font_width; b++)
            SetPixel(beg_x + b + 1, beg_y + z + 1, entry.bg);
    draw_char(key, beg_x, beg_y, entry.fg);
    log::info("GUI: entry_t: beg_x is %d and beg_y is %d and char is %c and fg is %d", beg_x, beg_y, key, entry.fg);

    Graphic::blit_changes();
}

void draw_entry(entry_t entry, coords_t coords)
{
    int x = coords.x + entry.coords.x;
    int y = coords.y + entry.coords.y;
    int max_w = coords.x + coords.w;
    int max_h = coords.y + coords.h;

    const int orig_x = x;
    const int orig_y = y;

    int *arr = (int *)entry.extra[0];
    char *keys_entered = (char *)entry.data[0];

    int limit_x = (orig_x + entry.coords.w) - orig_x;
    int limit_y = (orig_y + entry.coords.h) - orig_y;

    draw_rect(orig_x, orig_y, limit_x, limit_y, entry.bg);
    draw_empty_rect(orig_x, orig_y, entry.coords.w, entry.coords.h, entry.fg);

    if (x >= max_w || y >= max_h)
        return;

    for (int z = 0; z < arr[0]; z++)
    {
        draw_char(keys_entered[z], x, y, entry.fg);

        x += font_width;

        if (x >= max_w || y >= max_h)
        {
            return;
        }
    }
}

entry_t create_entry(auto win, int x, int y, int bg, int fg)
{
    entry_t entry;
    int arr[] = {0}; // keeps track of how many keys entered
    char keys[] = {0}; // keeps the keys entered

    entry.parent_id = win.id;
    entry.id = win.widget_count;
    entry.extra[0] = arr;
    entry.data[0] = keys;

    entry.coords.x = x;
    entry.coords.y = y;
    entry.coords.w = 10 * font_width + 2;
    entry.coords.h = font_height + 1;

    entry.bg = bg;
    entry.fg = fg;

    entry.mouse_click = default_mouse_click;
    entry.key_click = handle_entry_key;
    entry.draw_widget = draw_entry;

    return entry;
}

entry_t create_entry(auto win, int x, int y)
{
    return create_entry(win, x, y, DEFAULT_BG, DEFAULT_FG);
}
