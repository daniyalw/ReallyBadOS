#pragma once

#define MAX_WINDOWS 10

#include <gui/coords.h>
#include <gui/widget.h>

typedef struct
{
    char name[20];
    widget_t widgets[10];
    int widget_count = 0;
    coords_t coords;
    int id;

    bool dragged = false;
    bool null = false;

    int bg;
} window_t;

window_t windows[MAX_WINDOWS];
int windows_z[MAX_WINDOWS];
int window_count = 0;

window_t window_create(int x, int y, int bg, char *title, ...);
void win_draw(window_t win);

void add_new_window(window_t win);
void remove_window_z(int _z);
void remove_window_id(int id);
void add_window_at_location(int _z, int id);
void update_window_z(int id, int new_z);
