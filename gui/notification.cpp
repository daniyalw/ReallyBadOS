#include <font.h>
#include <video/graphics.h>

void gui_notification_string(char * string, int line)
{
    int root = height/2 + (font_height/2 + 3) + 3;
    draw_string(width/2 - BG_WIDTH_MINUS, line * (width * font_height + 1) + root, black, string);
}

int gui_notification(char * title, char * message)
{
    bool found_nl = false;

    for (int z = 0; z < std::len(message)/(TOTAL_CHARS_PER_LINE + 1); z++)
    {
        for (int b = 0; b < TOTAL_CHARS_PER_LINE; b++)
        {
            if (message[b+z*TOTAL_CHARS_PER_LINE] == '\n')
                found_nl = true;
        }

        if (!found_nl) return 1;
    }

    active_notifications.push_back(title, message);

    // title
    Graphic::draw_rect(width/2 - BG_WIDTH_MINUS, height/2 - (font_height/2 + TEXT_PADDING), BG_WIDTH, font_height + 6, black);
    // close button
    //Graphic::draw_line(CLOSE_BUTTON_BEGIN_W + 2, CLOSE_BUTTON_BEGIN_H + 2, CLOSE_BUTTON_END_W - 2, CLOSE_BUTTON_END_H - 2, Graphic::rgb(255, 0, 0));
    //Graphic::draw_line(CLOSE_BUTTON_END_W - 2, CLOSE_BUTTON_BEGIN_H + 2, CLOSE_BUTTON_BEGIN_W + 2, CLOSE_BUTTON_END_H - 2, Graphic::rgb(255, 0, 0));
    for (int z = 0; z < close_button_w; z++)
    {
        for (int b = 0; b < close_button_h; b++)
        {
            if (close_button_map[z+b*close_button_w] == 1)
            {
                Graphic::SetPixel(z + CLOSE_BUTTON_BEGIN_W + 10, (int)(b + CLOSE_BUTTON_BEGIN_H) + 7, Graphic::rgb(255, 0, 0));
            }
        }
    }
    // body
    Graphic::draw_rect(width/2 - BG_WIDTH_MINUS, height/2 + (font_height/2 + TEXT_PADDING), BG_WIDTH, (font_height + 3) * 6, white);
    draw_string(width/2 - BG_WIDTH_MINUS + TEXT_PADDING, height/2 - font_height/2, white, title);
    draw_string(width/2 - BG_WIDTH_MINUS + TEXT_PADDING, height/2 + (font_height/2 + TEXT_PADDING) + TEXT_PADDING, black, message);
    //gui_notification_string(message, 2);

    return 0;
}

bool did_click_notification(int x, int y)
{
    if (!active_notifications.size())
        return false;

    if (x >= CLOSE_BUTTON_BEGIN_W && x <= CLOSE_BUTTON_END_W)
        if (y >= CLOSE_BUTTON_BEGIN_H && y <= CLOSE_BUTTON_END_H)
            return true;

    return false;
}
