#include <kernel/mem/memory.h>
#include <kernel/mem/block.h>
#include <kernel/mem/free.h>
#include <kernel/mem/malloc.h>

void init_mem(auto mbd)
{
    uint32_t copy;
    uint32_t total;

    // the beginning of free memory
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
                    mem_blocks[block_count_mem] = m;
                    block_count_mem++;
                } else {
                    mem_t m = mem_blocks[block_count_mem-1];
                    m.size += mmmt->len_low;
                    mem_blocks[block_count_mem-1] = m;
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

#ifdef DEBUG
    printf("Total: %d\n", total);
#endif
}

#ifdef DEBUG
void print_lists()
{
    Kernel::system_log("\n\nPrinting 'mem_blocks' list:\n");

    for (int z = 0; z < block_count_mem; z++)
    {
        Kernel::system_log("Address: %d\nSize: %d\n\n", mem_blocks[z].addr, mem_blocks[z].size);
    }

    Kernel::system_log("\nPrinting 'used' list:\n");

    for (int z = 0; z < total_used; z++)
    {
        if (used[z].null == false)
            Kernel::system_log("Address: %d\nSize: %d\n\n", used[z].addr, used[z].size);
    }
}

void __mem_test()
{
    print_lists();

    uint32_t addr = malloc(10);
    uint32_t next = malloc(10);

    free((void *)addr);

    Kernel::system_log("%d\n", addr);
    Kernel::system_log("%d\n", next);

    print_lists();

    print_lists();

    Kernel::system_log("============\n");

    addr = malloc(10);

    Kernel::system_log("%d\n", addr);

    Kernel::system_log("============b\n");

    print_lists();

    addr = realloc((void *)addr, 20);

    Kernel::system_log("a============\n");

    Kernel::system_log("%d\n", addr);

    Kernel::system_log("============\n");

    print_lists();
}
#endif
