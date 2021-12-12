#pragma once

#include <map.h>

#define MEM_USED 0
#define MEM_FREE 1

// first int is memory location
// second int is size
int mem_beginning = 0x10000;
int total_mem;
std::map<int, int> memory_list;

void init_mem();
int get_free_block();
