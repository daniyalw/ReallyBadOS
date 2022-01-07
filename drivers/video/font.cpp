#pragma once
#include <string.h>
#include <font.h>

/*
MIT

Copyright © 2020 Remco123

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/

// test
void draw_char(char character, int x, int y, uint32_t color)
{
    int font_x, font_y;
    int count_x = 8;
    int count_y = 12;
    uint8_t shift_line;

    character &= 0x7F;

    for (font_y = 0; font_y < count_y; font_y++) {
        shift_line = font[font_y * 128 + character];
        for (font_x = 0; font_x < count_x; font_x++) {
            if (shift_line & 0x80)
                Graphic::SetPixel(font_x + x, font_y + y, color);

            shift_line <<= 1;
        }
    }
}

void draw_string(char *string, int x, int y, uint32_t color)
{
    int dx = x;
    for (; *string; dx += 8) {
        if (*(string) == '\n') {
            dx = x-8; y += 14; string++;
        }
        else if (*string == '\t') {
            dx += 8 * TAB_SIZE;
        }
        else
            draw_char(*(string++), dx, y, color);
    }
}

void draw_string(int x, int y, uint color, char *text, ...)
{
  char **arg = (char **) &text;
  int c;
  char buffer[20];
  int dx = x;

  arg++;

  while ((c = *text++) != 0)
    {
      if (c != '%')
      {
        if (c == '\n')
        {
            y += 14;
            dx = x;
        }
        else if (c == '\t')
        {
            for (int z = 0; z < 4; z++)
            {
                draw_char(' ', dx, y, color);
                dx += 8;
            }
        }
        else
        {
            draw_char(c, dx, y, color);
            dx += 8;
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
                draw_char(c, dx, y, color);
                dx += 8;
                break;
            case 'd':
            case 'u':
            case 'x':
              std::itoa (buffer, c, *((int *) arg++));
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
                draw_char(pad0 ? '0' : ' ', dx, y, color);
                dx += 8;
              }
              while (*p)
              {
                draw_char(*p, dx, y, color);
                dx += 8;
                *p++;
              }
              break;

            default:
              draw_char(*((int *) arg), dx, y, color);
              dx += 8;
              arg++;
              break;
            }
        }
    }
}
