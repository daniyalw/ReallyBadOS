#include <sys/mem/memory.h>
#include <sys/mem/block.h>
#include <sys/mem/malloc.h>

uint32_t malloc(int size)
{
    return get_free_block(size);
}
