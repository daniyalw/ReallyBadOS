// memory.cpp

unsigned int malloc(u32 size, int align, u32 *phys_addr) {

    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }

    if (phys_addr) *phys_addr = free_mem_addr;

    u32 ret = free_mem_addr;
    free_mem_addr += size;
    return ret;
}

void memcpy(unsigned char * source, unsigned char * destination, int nb)
{
    for (int z = 0; z < nb; z++)
    {
        destination[z] = source[z];
    }
}
