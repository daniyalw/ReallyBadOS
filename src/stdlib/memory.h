#pragma once

u32 free_mem_addr = 0x10000;
void memcpy(u8 *source, u8 *dest, int nb);
void memcpy(char *source, char *dest, int nb);
void memset(u8 *dest, u8 val, u32 len);
int memcmp(void* buf1, void* buf2, int count);
int memcmp(void* buf1, const void* buf2, int count);
u32 malloc(u32 size, int align, u32 *phys_addr);
u32 malloc(u32 size);
int get_mb(int kibibyte);
