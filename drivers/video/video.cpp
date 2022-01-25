#pragma once
#include <video/video.h>
#include <mouse/cursor.h>
#include <fs.h>

using namespace Filesystem;
using namespace Ramdisk;

void clear() {
    short * vidmem = (short *)0xb8000;

    for (int z = 0; z < 80; z++) {
        for (int b = 0; b < 25; b++) {
            vidmem[b*80+z] = 0x0F00 | ' ';
            written_on[z+b*80] = false;
            vga_back[b*80+z] = ' ';
        }
    }

    text_x = 0;
    text_y = 0;

    Kernel::update_hardware_cursor(text_x, text_y);
}

void next_char() {
    text_x++;
    if (text_x > 80) {
        text_x = 0;
        text_y++;

        if (scroll_on)
            scroll();
    }
}

void putchar(char text, int color) {
    if (text == '\n') {
        text_x = 0;
        text_y++;

        if (scroll_on)
            scroll();

        return;
    } else if (text == '\r') {
        text_x = 0;
        return;
    } else if (text == '\t') {
        text_x += 4;
        return;
    } else if (text == '\b') {
        if (text_x > 1)
        {
            // can't do text_x -= 2 since then the space two spaces behind text_x will turn empty and the space right
            // behind text_x will remain
            text_x--;
            if (text_x < 0) {
                for (int b = 0; b < 80; b++)
                {
                    if (!written_on[b+(text_y - 1)*80])
                    {
                        text_x = b;
                        break;
                    }
                }
                text_y--;
            }
            putchar(' ');
            text_x--;
            if (text_x < 0) {
                text_x = 0;
                text_y--;
            }
        }
        return;
    }

    short * vidmem = (short *)0xb8000;

    vidmem[text_x+text_y*80] = color | text;
    written_on[text_x+text_y*80] = true;
    vga_back[text_x+text_y*80] = text;
    next_char();
}

void putchar(char text) {
    if (!custom_color_on)
    {
        putchar(text, 0x0F00);
    }
    else
    {
        putchar(text, custom_color);
    }
}

void putchar_with_cursor_move(char text) {
    putchar(text);
    Kernel::set_hardware_cursor(text_y, text_x);
}

void scroll()
{
   uint8 attr = (0 << 4) | (15 & 0x0F);
   uint16 space = 0x20 | (attr << 8);
   short * vidmem = (short *)0xb8000;

   if (text_y > 24) {
       int i;
       for (i = 0*80; i < 24*80; i++)
       {
           vidmem[i] = vidmem[i+80];
           vga_back[i] = vga_back[i+80];
           written_on[i] = written_on[i+80];
       }

       for (i = 24*80; i < 25*80; i++)
       {
           vidmem[i] = space;
           vga_back[i] = space;
           written_on[i] = false;
       }
       text_y = 24;

       has_scrolled = true;
   }
}

void vaprintf(char *text, va_list va)
{
    char buffer[20];

    for (int z = 0; z < std::len(text); z++)
    {
        if (text[z] == '%')
        {
            z++;
            if (text[z] == 's')
            {
                char *str = (char *)va_arg(va, char*);

                for (int b = 0; b < std::len(str); b++)
                    putchar(str[b]);
            }
            else if (text[z] == 'c')
            {
                int ch = (int)va_arg(va, int);
                putchar(ch);
            }
            else if (text[z] == 'd')
            {
                int i = (int)va_arg(va, int);
                for (int b = 0; b < 20; b++) buffer[b] = 0;
                std::itoa(buffer, 'd', i);
                for (int b = 0; b < std::len(buffer); b++) putchar(buffer[b]);
            }
            else if (text[z] == 'x')
            {
                int arg = (int)va_arg(va, int);
                for (int b = 0; b < 20; b++) buffer[b] = 0;
                std::itoa(buffer, 'x', arg);
                for (int b = 0; b < std::len(buffer); b++) putchar(buffer[b]);
            }
        }
        else
        {
            putchar(text[z]);
        }
    }
    Kernel::set_hardware_cursor(text_y, text_x);
}

void printf(char *a, ...)
{
    va_list va;
    va_start(va, a);
    vaprintf(a, va);
    va_end(va);
}

void cprintf(int color, char *a, ...)
{
    custom_color_on = true;
    custom_color = color;

    va_list va;
    va_start(va, a);
    vaprintf(a, va);
    va_end(va);
}

void putchar_at(int x, int y, char c)
{
    short * vidmem = (short *)0xb8000;

    if (!custom_color_on)
        vidmem[x+y*80] = 0x0F00 | c;
    else
        vidmem[x+y*80] = custom_color | c;
}

void printf_centered(char *s, int line_no)
{
    int half = std::len(s)/2;
    int start = 40 - half;

    for (int z = 0; z < std::len(s); z++)
    {
        putchar_at(start, line_no, s[z]);
        start++;
    }
}

void warning(char *text, ...)
{
    int yellow = 0x0E00;
    Kernel::system_log("Warning: ");
    cprintf(yellow, "Warning: ");

    printf(text);
}

void error(char *text, ...)
{
    int red = 0x0400;
    Kernel::system_log("Error: ");
    cprintf(red, "Error: ");

    printf(text);
}

void info(char *text, ...)
{
    int light_blue = 0x0300;
    Kernel::system_log("Info: ");
    cprintf(light_blue, "Info: ");

    printf(text);
}

void write_vga(char *data)
{
    printf(data);
}

char * read_vga(char * data)
{
    return vga_back;
}

void init_vga()
{
    create_file("tty0", "dev", read_vga, write_vga);
}
