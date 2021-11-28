#pragma once

u32 free_mem_addr = 0x10000;
u32 malloc(u32 size, int align, u32 *phys_addr);
u32 malloc(u32 size);
