#pragma once

void set_new_used(mem_t m);
uint32_t get_free_block(int size);
void free_block(uint32_t addr, int size);
