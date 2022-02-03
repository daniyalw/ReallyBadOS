#pragma once

#include <sys/io.h>

static inline void outw(uint16_t portid, uint16_t value)
{
  asm("out %%ax, %%dx":: "a"(value), "d"(portid));
}

static inline void outb(uint16_t port, unsigned char value)
{
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void io_wait()
{
    outb(0x80, 0);
}

static inline u16 inw(uint32_t port)
{
    u16 data;
    __asm__ volatile("inw %w1, %w0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void outl(uint32_t port, u32 data)
{
    __asm__ volatile("outl %0, %w1" : : "a" (data), "Nd" (port));
}

static inline u32 inl(uint32_t port)
{
    u32 data;
    __asm__ volatile("inl %w1, %0" : "=a" (data) : "Nd" (port));
    return data;
}
