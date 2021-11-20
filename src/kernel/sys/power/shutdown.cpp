#pragma once
#include "shutdown.h"
#include "../time/timer.cpp"
#include "../../../drivers/video/graphics.cpp"
#include "../io.cpp"

void shutdown_os()
{
    acpiPowerOff();
}
