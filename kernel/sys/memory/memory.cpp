#pragma once

#include <kernel/memory/memory.h>
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
    uint32_t addr;
    int bz = 0;

    if(!(mbd->flags >> 6 & 0x1)) {
        PANIC("invalid memory map given by GRUB bootloader\n");
    }

    int i;
    for(i = 0; i < mbd->mmap_length;
        i += sizeof(multiboot_memory_map_t))
    {
        multiboot_memory_map_t* mmmt =
            (multiboot_memory_map_t*) (mbd->mmap_addr + i);

        #ifdef DEBUG
        printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
            mmmt->addr_low, mmmt->len_low, mmmt->size, mmmt->type);
        #endif

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            if (mmmt->addr_low != 0x100000) {
                addr = mmmt->addr_low;
                if (mmmt->addr_low == 0)
                    addr++; // so it doesn't do null

                total += mmmt->len_low;

                if (!last_av) {
                    mem_t m;
                    m.addr = addr;
                    m.size = mmmt->len_low;
                    bz++;
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
                continue;
            }
        } else {
            last_av = false;
            continue;
        }
    }

    out = total/BLOCK_SIZE;
    total = out * BLOCK_SIZE -1;

    total -= copy;

    total_memory = total;

    printf("Total: %d\n", total);
}
