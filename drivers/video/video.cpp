#pragma once
#include "video.h"
#include "../mouse/cursor.cpp"


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

void printf(int color, char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;

  while ((c = *format++) != 0)
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

          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
              itoa (buf, c, *((int *) arg++));
              p = buf;
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

void printf(char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;

  while ((c = *format++) != 0)
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

          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
              itoa (buf, c, *((int *) arg++));
              p = buf;
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
