#pragma once
#include <drivers/video/video.h>
#include <drivers/mouse/cursor.h>
#include <fs.h>

using namespace Filesystem;
using namespace Ramdisk;

int text_get_color(int backcolour, int forecolour)
{
    int attrib = (backcolour << 4) | (forecolour & 0x0F);
    return attrib;
}

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
    }
}

void putchar(char text) {
    if (text == '\n') {
        text_x = 0;
        text_y++;
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
    int color = 0x0F00;

    vidmem[text_x+text_y*80] = color | text;
    written_on[text_x+text_y*80] = true;
    vga_back[text_x+text_y*80] = text;
    next_char();
}

void putchar(char text, int color) {
    if (text == '\n') {
        text_x = 0;
        text_y++;
    } else if (text == '\r') {
        text_x = 0;
    } else if (text == '\t') {
        text_x += 4;
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
    }

    short * vidmem = (short *)0xb8000;

    vidmem[text_x+text_y*80] = color | text;
    written_on[text_x+text_y*80] = true;
    vga_back[text_x+text_y*80] = text;
    next_char();
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
        else if (c == '\b')
        {
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
    Kernel::set_hardware_cursor(text_y, text_x);
}

void printf(char *text, ...)
{
  char **arg = (char **) &text;
  int c;
  char buffer[20];

  if (!text)
  {
      for (int z = 0; z < std::len("(null)"); z++)
      {
          putchar("(null)"[z]);
      }

      return;
  }

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
        else if (c == '\b')
        {
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
                putchar(*arg++);
                break;
            case 'd':
                std::itoa (buffer, c, *((int *) arg++));
                p = buffer;
                goto string;
                break;
            case 'u':
            case 'x':
              putchar('0');
              putchar('x');
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
    Kernel::set_hardware_cursor(text_y, text_x);
}

void printf_centered(char * s, int line_no)
{
    int w = 80/2 - (std::len(s)/2);
    int i = line_no * 80 + w;
    short * vidmem = (short *)0xb8000;
    int c = 0x0F00;
    int l = std::len(s);
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
            vga_back[i+b] = s[z];
            next_char();
            b++;
        }
        z++;
    }
}

void warning(char *text, ...)
{
    int yellow = 0x0E00;
    Kernel::system_log("Warning: ");
    printf(yellow, "Warning: ");
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
        else if (c == '\b')
        {
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
                Kernel::system_log_char(' ');
                text_x--;
                if (text_x < 0) {
                    text_x = 0;
                    text_y--;
                }
            }
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
            Kernel::system_log_char(c);
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
                Kernel::system_log_char(c);
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
                putchar (pad0 ? '0' : ' ');
                Kernel::system_log_char(pad0 ? '0' : ' ');
              }
              while (*p)
              {
                putchar (*p);
                Kernel::system_log_char(*p);
                *p++;
              }
              break;

            default:
              putchar (*((int *) arg));
              Kernel::system_log_char(*(int *) arg);
              arg++;
              break;
            }
        }
    }
    Kernel::set_hardware_cursor(text_y, text_x);
}

void error(char *text, ...)
{
    int red = 0x0400;
    Kernel::system_log("Error: ");
    printf(red, "Error: ");
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
        else if (c == '\b')
        {
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
                Kernel::system_log_char(' ');
                text_x--;
                if (text_x < 0) {
                    text_x = 0;
                    text_y--;
                }
            }
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
            Kernel::system_log_char(c);
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
                Kernel::system_log_char(c);
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
                putchar (pad0 ? '0' : ' ');
                Kernel::system_log_char(pad0 ? '0' : ' ');
              }
              while (*p)
              {
                putchar (*p);
                Kernel::system_log_char(*p);
                *p++;
              }
              break;

            default:
              putchar (*((int *) arg));
              Kernel::system_log_char(*(int *) arg);
              arg++;
              break;
            }
        }
    }
    Kernel::set_hardware_cursor(text_y, text_x);
}

void info(char *text, ...)
{
    int light_blue = 0x0300;
    Kernel::system_log("Info: ");
    printf(light_blue, "Info: ");
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

            Kernel::system_log("\n");
        }
        else if (c == '\b')
        {
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
        }
        else if (c == '\t')
        {
            Kernel::system_log("    ");
            for (int z = 0; z < 4; z++) next_char();

            if (scroll_on)
                scroll();
        }
        else
        {
            putchar(c);
            Kernel::system_log_char(c);
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
                Kernel::system_log_char(c);
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
                putchar (pad0 ? '0' : ' ');
                Kernel::system_log_char(pad0 ? '0' : ' ');
              }
              while (*p)
              {
                putchar (*p);
                Kernel::system_log_char(*p);
                *p++;
              }
              break;

            default:
              putchar (*((int *) arg));
              Kernel::system_log_char(*(int *) arg);
              arg++;
              break;
            }
        }
    }
    Kernel::set_hardware_cursor(text_y, text_x);
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
