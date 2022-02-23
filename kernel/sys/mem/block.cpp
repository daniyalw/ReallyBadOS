#include <sys/mem/memory.h>
#include <sys/mem/block.h>

void set_new_used(mem_t m)
{
    for (int z = 0; z < total_used; z++)
    {
        if (used[z].null)
        {
            used[z] = m;
            return;
        }
    }

    used[total_used] = m;
    total_used++;
}

uint32_t get_free_block(int size)
{
    for (int z = 0; z < block_count_mem; z++)
    {
        if (mem_blocks[z].null)
        {
            continue;
        }
        else
        {
            mem_t cp = mem_blocks[z];

            if (cp.size > size)
            {
                cp.size -= size;
            }
            else if (cp.size == size)
            {
                cp.size = 0;
            }
            else
            {
                continue;
            }

            // if we reach here, the block is big enough
            uint32_t addr = cp.addr + cp.size;
            mem_blocks[z] = cp;

            mem_t m;
            m.addr = addr;
            m.size = size;
            set_new_used(m);

            return addr;
        }
    }

    return NULL;
}

void free_block(uint32_t addr, int size)
{
    // update the used list
    for (int z = 0; z < total_used; z++)
    {
        if (used[z].null) continue;
        else
        {
            if (used[z].addr == addr)
            {
                mem_t u = used[z];
                u.null = true;
                used[z] = u;
                break;
            }
        }
    }

    bool found_in_list = false;

    // tell the main list that the address is free for use
    for (int z = 0; z < block_count_mem; z++)
    {
        if (mem_blocks[z].null) continue;
        else
        {
            if (mem_blocks[z].addr + mem_blocks[z].size == addr)
            {
                found_in_list = true;
                mem_t b = mem_blocks[z];
                b.size += size;
                mem_blocks[z] = b;
                break;
            }
        }
    }

    if (found_in_list)
    {
        mem_t m;
        m.addr = addr;
        m.size = size;
        mem_blocks[block_count_mem] = m;
        block_count_mem++;
    }
}

uint32_t move_block_with_new_size(uint32 old, int newsize)
{
    mem_t block;

    for (int z = 0; z < total_used; z++)
    {
        if (used[z].null) continue;
        else
        {
            if (used[z].addr == old)
            {
                block = used[z];
            }
        }
    }

    if (newsize > block.size)
    {
        free_block(block.addr, block.size);

        uint32_t newaddr = get_free_block(newsize);

        if (newaddr == NULL)
        {
            return get_free_block(block.size);
        }

        for (uint32_t z = old; z < block.size; z++)
            ((short *)newaddr)[z] = ((short *)old)[z];

        return newaddr;
    }
    else
    {
        free_block(block.addr, block.size);

        uint32_t newaddr = get_free_block(newsize);

        if (newaddr == NULL)
        {
            // we should never come here, since we just freed enough space
            PANIC("NULL even after free()\n");
        }

        for (uint32_t z = old; z < newsize; z++)
        {
            ((short *)newaddr)[z] = ((short *)old)[z];
        }

        return newaddr;
    }

    PANIC("Surprise: We managed to find ourselves outside of an if-else loop that returns on both conditions...\n");

    return NULL; // we also shouldn't be here
}
