#include "shutdown.h"

void shutdown()
{
    outw(0x604, 0x2000);
}
