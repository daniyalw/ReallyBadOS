#include <sys/mem/memory.h>
#include <sys/mem/block.h>
#include <sys/mem/free.h>
#include <sys/mem/malloc.h>

void init_mem(auto mbd, uint32_t beginning_addr) {
    uint32_t copy;
    uint32_t total;

    // the beginning of free memory
    copy = kernel_end;

    int out = copy/BLOCK_SIZE;
    copy = out * BLOCK_SIZE + 1;

    bool last_av = false;
    uint32_t addr;
    int bz = 0;

    if (!(mbd->flags >> 6 & 0x1)) {
        log::error("invalid memory map given by GRUB bootloader\n");
    }

    int i;
    for(i = 0; i < mbd->mmap_length;
        i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt =
            (multiboot_memory_map_t*) (mbd->mmap_addr + i);

            DEBUG("TT: Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
                mmmt->addr_low, mmmt->len_low, mmmt->size, mmmt->type);

        total += mmmt->len_low;

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            if (mmmt->addr_low != 0) {
                if (mmmt->addr_low != 0x100000)
                    addr = mmmt->addr_low;
                else
                    addr = beginning_addr;

                total_usable += mmmt->len_low;

                DEBUG("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
                    addr, mmmt->len_low - (addr - 0x100000), mmmt->size, mmmt->type);

                mem_t m;
                m.addr = addr;
                m.size = mmmt->len_low - (addr - 0x100000);

                mem_blocks[block_count_mem] = m;
                block_count_mem++;

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

    DEBUG("Total: %d\n", total);
}

void print_lists() {
    log::warning("\n\nPrinting 'mem_blocks' list:\n");

    for (int z = 0; z < block_count_mem; z++) {
        log::warning("Address: %d\nSize: %d\n\n", mem_blocks[z].addr, mem_blocks[z].size);
    }

    log::warning("\nPrinting 'used' list:\n");

    for (int z = 0; z < total_used; z++) {
        if (used[z].null == false)
            log::warning("Address: %d\nSize: %d", used[z].addr, used[z].size);
    }
}

int get_total_allocated() {
    int total = 0;

    for (int z = 0; z < total_used; z++) {
        if (used[z].null == false) {
            total += used[z].size;
        }
    }

    return total;
}

int get_total_free() {
    return total_memory - get_total_allocated();
}

void __mem_test() {
    print_lists();

    uint32_t addr = malloc(10);
    uint32_t next = malloc(10);

    free((void *)addr);

    log::warning("%d", addr);
    log::warning("%d", next);

    print_lists();

    print_lists();

    log::warning("============");

    addr = malloc(10);

    log::warning("%d\n", addr);

    log::warning("============b");

    print_lists();

    addr = realloc((void *)addr, 20);

    log::warning("a============");

    log::warning("%d\n", addr);

    log::warning("============");

    print_lists();
}
