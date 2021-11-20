#pragma once

#include "memory.h"

void memcpy(u8 *source, u8 *dest, int nb) {
    int i;
    for (i = 0; i < nb; i++) {
        *(dest + i) = *(source + i);
    }
}

void memcpy(char *source, char *dest, int nb) {
    int i;
    for (i = 0; i < nb; i++) {
        *(dest + i) = *(source + i);
    }
}

void memcpy(u8 *source, char *dest, int nb) {
    int i;
    for (i = 0; i < nb; i++) {
        *(dest + i) = *(source + i);
    }
}

void memcpy(char *source, u8 *dest, int nb) {
    int i;
    for (i = 0; i < nb; i++) {
        *(dest + i) = *(source + i);
    }
}

void memset(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;

    for (; len != 0; len--)
    {
        *temp++ = val;
    }
}

int memcmp(void* buf1, void* buf2, int count) {
    if(!count)
        return(0);

    while(--count && *(char*)buf1 == *(char*)buf2 ) {
        buf1 = (char*)buf1 + 1;
        buf2 = (char*)buf2 + 1;
    }

    return(*((unsigned char*)buf1) - *((unsigned char*)buf2));
}

int memcmp(void* buf1, const void* buf2, int count) {
    if(!count)
        return(0);

    while(--count && *(char*)buf1 == *(char*)buf2 ) {
        buf1 = (char*)buf1 + 1;
        buf2 = (char*)buf2 + 1;
    }

    return(*((unsigned char*)buf1) - *((unsigned char*)buf2));
}

u32 malloc(u32 size, int align, u32 *phys_addr) {

    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr) *phys_addr = free_mem_addr;

    u32 ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}

u32 malloc(u32 size) {
    return malloc(size, 10, (u32 *)1);
}

int get_mb(int kibibyte) {
    int megabit = kibibyte * 0.008192;
    int megabyte = megabit/8;
    return megabyte;
}
