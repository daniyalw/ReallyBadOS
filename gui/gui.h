#pragma once

#include <drivers/video/graphics.h>

#define BG_TRANSPARENT -1
#define DEFAULT_BG Graphic::rgb(175, 175, 175)
#define DEFAULT_FG 0

using namespace Graphic;

#include <gui/coords.h>

void handle_mouse_click(coords_t coords, bool right, bool left, bool middle);
void handle_key_entry(char key);
