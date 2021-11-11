#pragma once
#include "shutdown.h"
#include "../time/timer.cpp"
#include "../../drivers/video/graphics.cpp"
#include "../io.cpp"

void shutdown_os()
{
    clear_screen();
    center_printf("Shutting down...", 0, height/2, width);
}
