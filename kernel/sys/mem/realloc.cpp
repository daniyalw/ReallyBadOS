#pragma once

#include <sys/mem/memory.h>
#include <sys/mem/block.h>

uint32_t realloc(void *buff, int newsize) {
    uint32_t old_addr = (uint32_t)buff;
    bool found = false;

    for (int z = 0; z < total_used; z++) {
        if (used[z].addr == old_addr) {
            found = true;
            break;
        }
    }

    if (found)
        return move_block_with_new_size(old_addr, newsize);
    else
        return NULL;
}
