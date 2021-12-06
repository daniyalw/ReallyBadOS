#pragma once

#include <map.h>

// first int is memory location
// second int is size
std::map<int, int> memory_free_locations;

void init_mem();
void free(int addr, int size);
int malloc(int size);
void wipe(int ad, int sz);
