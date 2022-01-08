#pragma once

#include <kernel/memory/memory.h>
#include <kernel/memory/block.h>
#include <kernel/memory/free.h>

void free(void *buff)
{
    uint32_t addr = (uint32_t)buff;
    int size;
    bool found = false;

    for (int z = 0; z < used.size(); z++)
    {
        if (used[z].addr == addr)
        {
            found = true;
            size = used[z].size;
            break;
        }
    }
    if (found)
        free_block(addr, size);
    else
        Kernel::system_log("Invalid buffer attempted to be freed.\n");

    //printf("Returning from free()\n");
}
