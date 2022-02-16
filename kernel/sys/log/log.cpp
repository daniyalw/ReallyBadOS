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
    char *out = vsprintf("", text, va);

    for (int z = 0; z < len(out); z++)
        Kernel::system_log_char(out[z]);
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
}

}
