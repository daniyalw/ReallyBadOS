#pragma once

#define MOUSE_RIGHT_CLICK 1
#define MOUSE_LEFT_CLICK 2
#define MOUSE_ENTER_HOVER 3
#define MOUSE_EXIT_HOVER 4

typedef struct
{
    int type;
    int mx, my;
} gui_event_t;
