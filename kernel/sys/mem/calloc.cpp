#include <kernel/mem/malloc.h>

uint32_t calloc(int number, int size)
{
    int total = number * size;
    return malloc(total);
}
