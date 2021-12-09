#pragma once
#include <kernel/log.h>
#include <kernel/serial.h>
#include <string.h>

namespace Kernel {

void system_log(char * data)
{
    serial_write_string(data);
    for (int z = 0; z < std::len(data); z++)
    {
        Kernel::buffer[Kernel::buffer_size] = data[z];
        Kernel::buffer_size++;
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
