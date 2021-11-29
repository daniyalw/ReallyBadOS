#pragma once
#include "log.h"

namespace Kernel {

void system_log(char * data)
{
    int size = len(data);

    for (int z = 0; z < size; z++)
    {
        sys_log[sys_log_size] = data[z];
        sys_log_size++;
    }
}

}
