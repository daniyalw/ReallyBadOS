#include <kernel/memory/memory.h>
#include <kernel/memory/block.h>
#include <kernel/memory/malloc.h>

uint32_t malloc(int size)
{
    uint32_t addr = get_block(size);
    return addr;
}
