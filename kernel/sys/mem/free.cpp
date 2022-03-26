#include <sys/mem/memory.h>
#include <sys/mem/block.h>
#include <sys/mem/free.h>

void free(void *buff) {
    uint32_t location = (uint32_t)buff;
    uint32_t size;

    bool found = false;

    for (int z = 0; z < total_used; z++) {
        if (used[z].addr == location) {
            found = true;
            size = used[z].size;
            break;
        }
    }

    if (!found) {
        log::error("Attempted to free invalid buffer!");
        return;
    }

    free_block(location, size);
}
