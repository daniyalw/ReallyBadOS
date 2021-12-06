#pragma once

#include <memory.h>

int malloc(int size)
{
    for (int z = 0; z < memory_free_locations.size(); z++)
    {
        if (memory_free_locations.get_value(z) == size)
        {
            int mem_location = memory_free_locations.get_key(z);
            memory_free_locations.remove(z);
            return mem_location;
        }
        else if (memory_free_locations.get_value(z) > size)
        {
            int loc = memory_free_locations.get_key(z);
            int sz = memory_free_locations.get_value(z);
            memory_free_locations.replace(loc+size, sz-size, memory_free_locations.get_first_pos(loc));
            return loc;
        }
    }

    return NULL;
}

void wipe(int ad, int sz)
{
    short * a = (short *)ad;

    for (int z = 0; z < sz; z++)
    {
        a[z] = 0;
    }
}

void free(int addr, int size)
{
    wipe(addr, size);

    for (int z = 0; z < memory_free_locations.size(); z++)
    {
        if ((memory_free_locations.get_key(z)-size) == addr)
        {
            memory_free_locations.replace(memory_free_locations.get_key(z)-size, memory_free_locations.get_value(z)+size, memory_free_locations.get_first_pos(memory_free_locations.get_key(z)));
            return;
        }
    }

    memory_free_locations.push_back(addr, size);
}

void init_mem()
{
    memory_free_locations.push_back(0x10000, 400);
}
