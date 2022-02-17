#pragma once

#include <va_list.h>

namespace log
{
    void info(char *data, ...);
    void error(char *data, ...);
    void warning(char *data, ...);
}

namespace Kernel
{
    void init_logging();
}
