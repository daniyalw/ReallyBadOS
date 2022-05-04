#pragma once

#include <drivers/video/graphics.h>
#include "object.h"

#define BG_TRANSPARENT -1
#define DEFAULT_BG Graphic::rgb(175, 175, 175)
#define DEFAULT_FG 0
#define DEFAULT_FONT 16
#define TITLEBAR_HEIGHT font_height + 4
#define CLOSE_WIN_X TITLEBAR_HEIGHT
#define HIDE_WIN_X CLOSE_WIN_X + CLOSE_WIN_X
#define HIDE_WIN_Y CLOSE_WIN_X

#define EVENT_KEY 1
#define EVENT_MOUSE_LEFT 2
#define EVENT_MOUSE_RIGHT 3
#define EVENT_MOUSE_MIDDLE 4
#define EVENT_MOUSE_HOVER 5
#define EVENT_MOUSE_EXIT 6

using namespace Graphic;

#include <gui/coords.h>

void handle_mouse_click(UI::Coords coords, bool right, bool left, bool middle);
void handle_key_entry(char key);

inline UI::Event *create_event();
inline int get_mouse_click_type(bool right, bool left, bool middle);
