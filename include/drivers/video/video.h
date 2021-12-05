#pragma once

#define TERMINAL_HEIGHT 25
#define TERMINAL_WIDTH 80

bool scroll_on = true;
bool written_on[80*25];
void clear();
void next_char();
void putchar(char text);
void print(char * text);
void scroll();
void printf(char *text, ...);
void printf_centered(char * s, int line_no);
void warning(char * warn);
void info(char * s);
void error(char * err);

struct output
{
    void operator<<(char * text)
    {
        print(text);
    }
};

output cout;
