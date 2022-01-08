#pragma once

void save_block(int z, mem_t mem);
uint32_t get_block(int size);
void free_block(uint32_t addr, int size);

#ifdef DEBUG
void print_list();
#endif
