#pragma once
#include "shutdown.h"
#include "../time/timer.cpp"
#include "../../../drivers/video/graphics.cpp"
#include "../io.cpp"

namespace Kernel {

void shutdown_os()
{
    Kernel::acpiPowerOff();
}

}
