#pragma once

#include "memory.h"

u32 malloc(u32 size, int align, u32 *phys_addr) {

    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr) *phys_addr = free_mem_addr;

    u32 ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}

u32 malloc(u32 size) {
    return malloc(size, 10, (u32 *)1);
}
