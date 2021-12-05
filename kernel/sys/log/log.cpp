#pragma once
#include <kernel/log.h>
#include <kernel/serial.h>

namespace Kernel {

void system_log(char * data)
{
    serial_write_string(data);
}

}
