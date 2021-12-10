#pragma once
#include <kernel/log.h>
#include <kernel/serial.h>
#include <string.h>

namespace Kernel {

void system_log_char(char ch)
{
    output_serial_char(ch);

    Kernel::buffer[Kernel::buffer_size] = ch;
    Kernel::buffer_size++;
}

void system_log(char *text, ...)
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
            serial_write_string("\n");
        }
        else if (c == '\t')
        {
            serial_write_string("    ");
        }
        else
        {
            system_log_char(c);
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
                system_log_char(c);
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
                system_log_char (pad0 ? '0' : ' ');
              }
              while (*p)
              {
                system_log_char (*p);
                *p++;
              }
              break;

            default:

              arg++;
              break;
            }
        }
    }
}


char * get_log(char * buff)
{
    for (int z = 0; z < Kernel::buffer_size; z++)
    {
        buff[z] = Kernel::buffer[z];
    }

    return buff;
}

}
