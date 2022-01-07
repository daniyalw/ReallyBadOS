#pragma once

#include <memory.h>
#include <string.h>
#include <panic.h>

void init_mem(auto mbd)
{
    uint32_t copy;
    uint32_t total;

    copy = kernel_end;

    int out = copy/BLOCK_SIZE;
    copy = out * BLOCK_SIZE + 1;

    bool last_av = false;

    if(!(mbd->flags >> 6 & 0x1)) {
        PANIC("invalid memory map given by GRUB bootloader\n");
    }

    int i;
    for(i = 0; i < mbd->mmap_length;
        i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t* mmmt =
            (multiboot_memory_map_t*) (mbd->mmap_addr + i);

        printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
            mmmt->addr_low, mmmt->len_low, mmmt->size, mmmt->type);

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            if (mmmt->addr_low != 0x100000) {
                total += mmmt->len_low;

                if (!last_av) {
                    mem_t m;
                    m.addr = mmmt->addr_low;
                    m.size = mmmt->len_low;
                    mlist.push_back(m);
                } else {
                    mem_t m = mlist.last();
                    m.size += mmmt->len_low;
                    mlist.pop();
                    mlist.push_back(m);
                }

                last_av = true;
            } else {
                last_av = false;
            }
        } else {
            last_av = false;
        }
    }

    out = total/BLOCK_SIZE;
    total = out * BLOCK_SIZE -1;

    total -= copy;

    total_memory = total;
}

void print_list()
{
    printf("\nSize of list: %d", mlist.size());
    for (int z = 0; z < mlist.size(); z++)
    {
        printf("\nAddr: %d | Size: %d", mlist.get(z).addr, mlist.get(z).size);
    }
}
