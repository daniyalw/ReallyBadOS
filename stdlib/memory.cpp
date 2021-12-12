#pragma once

#include <memory.h>

namespace std {

int get_free_block() {
    int location;

    for (int z = 0; z < memory_list.size(); z++)
    {
        location = memory_list.get_key(z);

        if (memory_list.get_value(z) == MEM_FREE)
        {
            memory_list.replace(location, MEM_USED, z);
            return location;
        }
    }

    return NULL;
}

int get_free_blocks(int blocks) {
    if (blocks == 0 || blocks == 1)
        return 1;

    bool potential = false;
    int together = 0;
    int loc = NULL;
    int potential_location = NULL;

    for (int z = 0; z < memory_list.size(); z++)
    {
        loc = memory_list.get_key(z);

        if (memory_list.get_value(z) == MEM_FREE)
        {
            potential = true;
            together++;

            if (together == blocks)
            {
                return potential_location;
            }
            else if (together == 1)
            {
                potential_location = loc;
            }
        }
        else
        {
            potential = false;
            together = 0;
            potential_location = NULL;
        }
    }

    return 2;
}

void free_block(int location) {
    for (int z = 0; z < memory_list.size(); z++)
    {
        if (memory_list.get_key(z) == location)
        {
            if (memory_list.get_value(z) != MEM_USED)
            {
                return;
            }
            else
            {
                memory_list.replace(location, MEM_FREE, z);
            }
        }
    }
}

void free_blocks(int location, int blocks) {
    for (int z = 0; z < size; z++)
    {
        free_block(location + (z * 4000));
    }
}

int malloc(int size) {
    int blocks_needed = size/4000;

    return get_free_blocks(blocks_needed);
}

void free(int addr, int size) {
    int blocks_to_free = size/4000;

    free_blocks(addr, blocks_to_free);
}

}

namespace Kernel {

void init_mem(multiboot_info_t * mbd)
{
    int total = 0;

    if (!(mbd->flags >> 6 & 0x1)) {
        error("invalid memory map given by GRUB!");
        return;
    }

    for (int z = 0; z < mbd->mmap_length;  z += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt =  (multiboot_memory_map_t*) (mbd->mmap_addr + z);
        total += mmmt->len_low;
    }

    total_mem = total;
    int count = 0;

    for (int z = mem_beginning; z < total; z += 4000)
    {
        memory_list.push_back(z, MEM_FREE);
        count++;
    }
}

}
