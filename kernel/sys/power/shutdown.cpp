#pragma once
#include <sys/power/power.h>

namespace Kernel {

void shutdown_os()
{
    Kernel::system_log("Shutting down...\n");
    
    // bochs and older qemu
    outw(0xB004, 0x2000);

    // newer qemu
    outw(0x604, 0x2000);

    // virtualbox
    outw(0x4004, 0x3400);
}

}
