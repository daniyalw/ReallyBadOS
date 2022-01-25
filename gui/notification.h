#pragma once

#include <font.h>

#define BG_WIDTH 400
#define BG_WIDTH_MINUS BG_WIDTH/2
#define TEXT_PADDING 3
#define TOTAL_CHARS_PER_LINE (BG_WIDTH - TEXT_PADDING * 2)/font_width
#define CLOSE_BUTTON_BEGIN_W width/2 + BG_WIDTH_MINUS - TEXT_PADDING * 10
#define CLOSE_BUTTON_END_W width/2 + BG_WIDTH_MINUS
#define CLOSE_BUTTON_BEGIN_H height/2 - (font_height/2 + TEXT_PADDING)
#define CLOSE_BUTTON_END_H height/2 - (font_height/2 + TEXT_PADDING) + font_height + 6

std::map<char *, char *> active_notifications;

int close_button_map[] = {
    1, 0, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 1, 0,
    0, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
    0, 0, 1, 0, 1, 0, 0,
    0, 1, 0, 0, 0, 1, 0,
    1, 0, 0, 0, 0, 0, 1,
};
int close_button_w = 7;
int close_button_h = 7;

bool did_click_notification(int x, int y);
int gui_notification(char * title, char * message);
void gui_notification_string(char * string, int line);
