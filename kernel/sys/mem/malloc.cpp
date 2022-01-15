#include <kernel/mem/memory.h>
#include <kernel/mem/block.h>
#include <kernel/mem/malloc.h>

uint32_t malloc(int size)
{
    return get_free_block(size);
}
