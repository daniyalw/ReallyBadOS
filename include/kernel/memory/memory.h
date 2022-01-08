#pragma once

#include <map.h>

#define MEM_USED 0
#define MEM_FREE 1

// 4k
#define BLOCK_SIZE 4096

typedef struct
{
    uint32_t addr;
    int size;
} mem_t;

std::list<mem_t> mlist;
std::list<mem_t> used;
uint32_t total_memory;

void init_mem(auto mbd);
