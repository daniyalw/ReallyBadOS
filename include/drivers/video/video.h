#pragma once

#define TERMINAL_HEIGHT 25
#define TERMINAL_WIDTH 80

bool scroll_on = true;
bool written_on[80*25];
void clear();
void next_char();
void putchar(char text);
void scroll();
void printf(char *text, ...);
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
