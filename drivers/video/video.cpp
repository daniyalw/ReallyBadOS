#pragma once
#include "video.h"
#include "../mouse/cursor.cpp"

int text_get_color(int backcolour, int forecolour)
{
    int attrib = (backcolour << 4) | (forecolour & 0x0F);
    return attrib;
}

void clear() {
    short * vidmem = (short *)0xb8000;

    for (int z = 0; z < 80; z++) {
        for (int b = 0; b < 25; b++)
            vidmem[b*80+z] = 0x0F00 | ' ';
    }

    text_x = 0;
    text_y = 0;
}

void next_char() {
    text_x++;
    if (text_x > 80) {
        text_x = 0;
        text_y++;
    }
}

void putchar(char text) {
    short * vidmem = (short *)0xb8000;
    int color = 0x0F00;

    vidmem[text_x+text_y*80] = color | text;
    next_char();
}

void putchar(char text, int color) {
    short * vidmem = (short *)0xb8000;

    vidmem[text_x+text_y*80] = color | text;
    next_char();
}

void print(char * text) {
    for (int z = 0; z < len(text); z++) {
        putchar(text[z]);
    }
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
       }

       for (i = 24*80; i < 25*80; i++)
       {
           vidmem[i] = space;
       }
       text_y = 24;
   }
}

void printf(int color, char *text, ...)
{
  char **arg = (char **) &text;
  int c;
  char buffer[20];

  arg++;

  while ((c = *text++) != 0)
    {
      if (c != '%')
      {
        if (c == '\n')
        {
            text_x = 0;
            text_y++;

            if (scroll_on)
                scroll();
        }
        else if (c == '\t')
        {
            for (int z = 0; z < 4; z++) next_char();

            if (scroll_on)
                scroll();
        }
        else
        {
            putchar(c, color);
        }
      }
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;

          c = *text++;
          if (c == '0')
            {
              pad0 = 1;
              c = *text++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *text++;
            }

          switch (c)
            {
            case 'c':
                putchar(c);
                break;
            case 'd':
            case 'u':
            case 'x':
              itoa (buffer, c, *((int *) arg++));
              p = buffer;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
              {
                putchar (pad0 ? '0' : ' ', color);
              }
              while (*p)
              {
                putchar (*p++, color);
              }
              break;

            default:
              putchar (*((int *) arg++), color);
              break;
            }
        }
    }
    set_hardware_cursor(text_y, text_x);
}

void printf(char *text, ...)
{
  char **arg = (char **) &text;
  int c;
  char buffer[20];

  arg++;

  while ((c = *text++) != 0)
    {
      if (c != '%')
      {
        if (c == '\n')
        {
            text_x = 0;
            text_y++;

            if (scroll_on)
                scroll();
        }
        else if (c == '\t')
        {
            for (int z = 0; z < 4; z++) next_char();

            if (scroll_on)
                scroll();
        }
        else
        {
            putchar(c);
        }
      }
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;

          c = *text++;
          if (c == '0')
            {
              pad0 = 1;
              c = *text++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *text++;
            }

          switch (c)
            {
            case 'c':
                putchar(c);
                break;
            case 'd':
            case 'u':
            case 'x':
              itoa (buffer, c, *((int *) arg++));
              p = buffer;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
              {
                putchar (pad0 ? '0' : ' ');
              }
              while (*p)
              {
                putchar (*p++);
              }
              break;

            default:
              putchar (*((int *) arg++));
              break;
            }
        }
    }
    set_hardware_cursor(text_y, text_x);
}

void printf_centered(char * s, int line_no)
{
    int w = 80/2 - (len(s)/2);
    int i = line_no * 80 + w;
    short * vidmem = (short *)0xb8000;
    int c = 0x0F00;
    int l = len(s);
    int z = 0;
    int b = 0;

    while (s[z] != '\0')
    {
        if (s[z] == '\n')
        {
            text_x = 0;
            text_y++;
        }
        else
        {
            vidmem[i+b] = c | s[z];
            next_char();
            b++;
        }
        z++;
    }
}

void warning(char * warn)
{
    int yellow = 0x0E00;
    printf(yellow, "warning: ");
    printf(warn);
}

void error(char * err)
{
    int red = 0x0400;
    printf(red, "error: ");
    printf(err);
}

void info(char * s)
{
    int light_blue = 0x0300;
    printf(light_blue, "info: ");
    printf(s);
}
