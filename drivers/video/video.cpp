#pragma once
#include <video/video.h>
#include <mouse/cursor.h>
#include <filesystem/ramdisk.h>
#include <sys/serial.h>

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
    short * vidmem = (short *)0xb8000;


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
        for (int z = 0; z < 4; z++) putchar(' ');
        return;
    } else if (text == '\b') {
        text_x--;
        vidmem[text_x+text_y*80] = color | ' ';
        written_on[text_x+text_y*80] = true;
        vga_back[text_x+text_y*80] = ' ';

        return;
    }

    vidmem[text_x+text_y*80] = color | text;
    written_on[text_x+text_y*80] = true;
    vga_back[text_x+text_y*80] = text;
    next_char();
}

char * vsnprintf(char *s, int max, char *format, va_list va)
{
    int sz = 0;
    char buffer[20];

    for (int z = 0; z < max; z++)
    {
        if (format[z] != '%')
        {
            s[sz] = format[z];
            sz++;
            continue;
        }

        z++;

        if (format[z] == 's')
        {
            char *str = (char *)va_arg(va, char*);

            for (int b = 0; b < len(str); b++)
            {
                s[sz] = str[b];
                sz++;
            }
        }
        else if (format[z] == 'c')
        {
            int ch = (int)va_arg(va, int);
            s[sz] = ch;
            sz++;
        }
        else if (format[z] == 'd')
        {
            int i = (int)va_arg(va, int);
            for (int b = 0; b < 20; b++) buffer[b] = 0;
            itoa(buffer, 'd', i);
            for (int b = 0; b < len(buffer); b++)
            {
                s[sz] = buffer[b];
                sz++;
            }
        }
        else if (format[z] == 'x')
        {
            int arg = (int)va_arg(va, int);
            for (int b = 0; b < 20; b++) buffer[b] = 0;
            itoa(buffer, 'x', arg);
            for (int b = 0; b < len(buffer); b++)
            {
                s[sz] = buffer[b];
                sz++;
            }
        }
        else
        {
            s[sz] = '%';
            sz++;
            s[sz] = format[z];
            sz++;
        }
    }

    s[sz] = 0;

    return s;
}

char * vsprintf(char *s, char *format, va_list va)
{
    return vsnprintf(s, len(format), format, va);
}

void vprintf(char *f, va_list va)
{
    printf(vsprintf("", f, va));
}

void sprintf(char *s, char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vsprintf(s, fmt, va);
    va_end(va);
}

void snprintf(char *s, int max, char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vsnprintf(s, max, fmt, va);
    va_end(va);
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
           vga_back[i] = ' ';
           written_on[i] = false;
       }
       text_y = 24;
   }
}

void printf(char *a, ...)
{
    va_list va;
    va_start(va, a);
    char * out = vsprintf("", a, va);
    va_end(va);

    for (int z = 0; z < strlen(out); z++)
        putchar(out[z]);

    Kernel::update_hardware_cursor(text_x, text_y);
}

void cprintf(int color, char *a, ...)
{
    custom_color_on = true;
    custom_color = color;

    va_list va;
    va_start(va, a);
    vprintf(a, va);
    va_end(va);

    custom_color_on = false;
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
    int half = len(s)/2;
    int start = 40 - half;

    for (int z = 0; z < len(s); z++)
    {
        putchar_at(start, line_no, s[z]);
        start++;
    }
}

void write_vga(fs_node node, int offset, int size, char *data)
{
    printf(data);
}

char * read_vga(fs_node node, int offset, int size, char * data)
{
    return vga_back;
}

void write_error(fs_node node, int offset, int size, char *data)
{
    p_error(data);
}

char *read_error(fs_node node, int offset, int size, char *data)
{
    UNUSED(data);
    // not implemented
    return NULL;
}

fs_node stdout;
fs_node stderr;

void init_error()
{
    create_file("stderr", "/dev/", read_error, write_error);
    stderr = find_node(find_id("/dev/stderr"));
}

void init_vga()
{
    create_file("stdout", "/dev/", read_vga, write_vga);
    stdout = find_node(find_id("/dev/stdout"));
    init_error();
}
