#include <gui/button.h>
#include <gui/window.h>

void draw_button(button_t button, coords_t coords)
{
    int x = coords.x + button.coords.x;
    const int orig_x = x;
    x += BUTTON_PIXEL_PADDING;
    int y = coords.y + button.coords.y;
    const int orig_y = y;
    y += BUTTON_PIXEL_PADDING;
    int max_w = coords.x + coords.w;
    int max_h = coords.y + coords.h;
    char *text = (char *)button.data[0];

    int limit_x = (orig_x + button.coords.w) - orig_x;
    int limit_y = (orig_y + button.coords.h) - orig_y;

    if (x >= max_w || y >= max_h)
        return;

    draw_rect(orig_x, orig_y, limit_x, limit_y, button.bg);
    draw_empty_rect(orig_x, orig_y, button.coords.w, button.coords.h, button.fg);

    for (int z = 0; z < strlen(text); z++)
    {
        draw_char(text[z], x, y, button.fg);

        x += font_width;

        if (x >= max_w || y >= max_h)
        {
            return;
        }
    }
}

button_t create_button(auto win, char *text, int x, int y, auto callback, int bg, int fg)
{
    button_t button;

    button.id = win.widget_count;
    button.parent_id = win.id;
    button.data[0] = text;

    button.coords.x = x;
    button.coords.y = y;
    button.coords.w = (strlen(text) * font_width) + (2 * BUTTON_PIXEL_PADDING); // 3px padding on each side
    button.coords.h = font_height + (2 * BUTTON_PIXEL_PADDING);

    button.bg = bg;
    button.fg = fg;

    button.mouse_click = callback;
    button.key_click = default_key_click;
    button.draw_widget = draw_button;

    return button;
}

button_t create_button(auto win, char *text, int x, int y, auto callback)
{
    return create_button(win, text, x, y, callback, DEFAULT_BG, DEFAULT_FG);
}
