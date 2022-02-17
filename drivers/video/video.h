#pragma once

#include <va_list.h>

#define TERMINAL_HEIGHT 25
#define TERMINAL_WIDTH 80

#define REG_SERIAL_OUT "\033[32;40m"

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
void snprintf(char *s, int max, char *fmt, ...);
void sprintf(char *s, char *fmt, ...);
void vprintf(char *f, va_list va);
char * vsprintf(char *s, char *format, va_list va);
char * vsnprintf(char *s, int max, char *format, va_list va);
void scroll();
void vaprintf(char *text, va_list va);
void printf(char *a, ...);
void cprintf(int color, char *a, ...);
void printf_centered(char * s, int line_no);

struct output
{
    void operator<<(char * text)
    {
        printf(text);
    }
};

output cout;
