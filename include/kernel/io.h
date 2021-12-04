#pragma once

static inline void outw(unsigned short portid, unsigned short value);
static inline void outb(unsigned short port, unsigned char value);
static inline unsigned char inb(unsigned short port);
static inline void io_wait(void);
static inline u16 inw(uint port);
static inline void outl(uint port, u32 data);
static inline u32 inl(uint port);
