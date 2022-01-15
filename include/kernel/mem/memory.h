#pragma once

typedef struct
{
    uint32_t addr;
    int size;
    bool null = false;

    bool operator!() { return null; }
} mem_t;

#define BLOCK_SIZE 4096

mem_t mem_blocks[128];
int block_count_mem = 0;
mem_t used[128];
int total_used = 0;

int total_memory;
