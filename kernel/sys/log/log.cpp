#pragma once
#include <sys/log/log.h>
#include <sys/serial.h>
#include <string.h>

namespace Kernel {

void system_log_char(char ch)
{
    Kernel::output_serial_char(ch);

    Kernel::buffer[Kernel::buffer_size] = ch;
    Kernel::buffer_size++;
}

void system_log_string(char *text, va_list va)
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
                    Kernel::system_log_char(str[b]);
            }
            else if (text[z] == 'c')
            {
                int ch = (int)va_arg(va, int);
                Kernel::system_log_char(ch);
            }
            else if (text[z] == 'd')
            {
                int i = (int)va_arg(va, int);
                for (int b = 0; b < 20; b++) buffer[b] = 0;
                std::itoa(buffer, 'd', i);
                for (int b = 0; b < std::len(buffer); b++) Kernel::system_log_char(buffer[b]);
            }
            else if (text[z] == 'x')
            {
                int arg = (int)va_arg(va, int);
                for (int b = 0; b < 20; b++) buffer[b] = 0;
                std::itoa(buffer, 'x', arg);
                for (int b = 0; b < std::len(buffer); b++) Kernel::system_log_char(buffer[b]);
            }
        }
        else
        {
            Kernel::system_log_char(text[z]);
        }
    }
}

void system_log(char * text, ...)
{
    va_list va;
    va_start(va, text);
    system_log_string(text, va);
    va_end(va);
}

char * get_log(char * buff)
{
    for (int z = 0; z < Kernel::buffer_size; z++)
    {
        buff[z] = Kernel::buffer[z];
    }

    return buff;
}

void log(char * data)
{
    Kernel::system_log(data);
}

void init_logging()
{
    create_file("tty1", "dev", Kernel::get_log, Kernel::log);
}

}
