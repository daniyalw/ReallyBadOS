#pragma once
#include <sys/power/power.h>

namespace Kernel {

void shutdown_os()
{
    log::warning("Shutting down...");

    // bochs and older qemu
    Kernel::IO::outw(0xB004, 0x2000);

    // newer qemu
    Kernel::IO::outw(0x604, 0x2000);

    // virtualbox
    Kernel::IO::outw(0x4004, 0x3400);
}

}
