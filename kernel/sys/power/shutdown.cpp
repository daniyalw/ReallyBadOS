#pragma once
#include <kernel/acpi.h>
#include <kernel/power.h>

namespace Kernel {

void shutdown_os()
{
    Kernel::acpiPowerOff();
}

}
