#include <kernel/mem/malloc.h>

uint32_t calloc(int number, int size)
{
    int total = number * size;
    uint32_t addr = malloc(total);

    for (int z = 0; z < total; z++)
    {
        ((uint32_t *)addr)[z] = 0;
    }

    return addr;
}
