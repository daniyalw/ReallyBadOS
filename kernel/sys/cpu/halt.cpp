#include <kernel/halt.h>

namespace Kernel {

void halt_system()
{
    asm volatile("hlt");
}

}
