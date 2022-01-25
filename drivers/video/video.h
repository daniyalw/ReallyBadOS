#pragma once

#include <va_list.h>

#define TERMINAL_HEIGHT 25
#define TERMINAL_WIDTH 80

bool scroll_on = true;
bool written_on[80*25];
char vga_back[80*25];
bool to_clear = true;

bool custom_color_on = false;
int custom_color;

bool has_scrolled = false;

void clear();
void next_char();
void putchar(char text, int color);
void putchar_with_cursor_move(char text);
void putchar(char text);
void scroll();
void vaprintf(char *text, va_list va);
void printf(char *a, ...);
void cprintf(int color, char *a, ...);
void printf_centered(char * s, int line_no);
void warning(char * text, ...);
void info(char * text, ...);
void error(char * text, ...);

struct output
{
    void operator<<(char * text)
    {
        printf(text);
    }
};

output cout;
