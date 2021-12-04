#pragma once
#include <kernel/log.h>

namespace Kernel {

void system_log(char * data)
{
    int size = len(data);

    for (int z = 0; z < size; z++)
    {
        Kernel::sys_log[Kernel::sys_log_size] = data[z];
        Kernel::sys_log_size++;
    }
}

}
