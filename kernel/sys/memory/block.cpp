#pragma once

#include <kernel/memory/memory.h>
#include <string.h>
#include <panic.h>

void save_block(int z, mem_t mem)
{
    mlist.replace(mem, z);
}

uint32_t get_block(int size)
{
    for (int z = 0; z < mlist.size(); z++)
    {
        uint32_t addr;

        mem_t m = mlist[z];
        addr = m.addr + m.size - size;

        m.size -= size;

        mem_t u;
        u.addr = addr;
        u.size = size;
        used.push_back(u);

        if (m.size == 0)
        {
            mlist.remove(z);
        }
        else if (m.size < 0)
        {
            continue;
        }
        else
        {
            save_block(z, m);
        }

        return addr;
    }

    return -1;
}

void free_block(uint32_t addr, int size)
{
    bool found;

    for (int z = 0; z < used.size(); z++)
        if (used[z].addr == addr)
        {
            found = true;
            used.remove(z);
        }

    if (!found) return;

    for (int z = 0; z < mlist.size(); z++)
    {
        if ((mlist[z].addr + mlist[z].size - size) == addr)
        {
            mem_t m = mlist[z];
            m.size += size;
            save_block(z, m);
            return;
        }
    }

    mem_t m;
    m.addr = addr;
    m.size = size;
    mlist.push_back(m);
    return;
}

#ifdef DEBUG
void print_list()
{
    printf("\nSize of list: %d", mlist.size());
    for (int z = 0; z < mlist.size(); z++)
    {
        printf("\nAddr: %d | Size: %d", mlist.get(z).addr, mlist.get(z).size);
    }
}
#endif
