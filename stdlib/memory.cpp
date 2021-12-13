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

    int together = 0;
    int loc = NULL;
    int potential_location = NULL;
    int potential_z = NULL;

    for (int z = 0; z < memory_list.size(); z++)
    {
        loc = memory_list.get_key(z);

        if (memory_list.get_value(z) == MEM_FREE)
        {
            together++;

            if (together == blocks)
            {
                for (int b = 0; b < blocks; b++)
                {
                    memory_list.replace_value(MEM_USED, potential_z+b);
                }
                return potential_location;
            }
            else if (together == 1)
            {
                potential_location = loc;
                potential_z = z;
            }
        }
        else
        {
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
                return;
            }
        }
    }
}

void free_blocks(int location, int blocks) {
    for (int z = 0; z < blocks; z++)
    {
        free_block(location + (z * BLOCK_SIZE));
    }
}

int malloc(int size) {
    int blocks_needed = size/BLOCK_SIZE;

    return get_free_blocks(blocks_needed);
}

void free(int addr, int size) {
    int blocks_to_free = size/BLOCK_SIZE;

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

    for (int z = mem_beginning; z < total; z += BLOCK_SIZE)
    {
        memory_list.push_back(z, MEM_FREE);
        count++;
    }
}

}
