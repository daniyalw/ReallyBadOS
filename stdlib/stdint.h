#pragma once

typedef unsigned int   u32;
typedef          int   s32;
typedef unsigned short u16;
typedef          short s16;
typedef unsigned char  u8;
typedef          char  s8;

typedef unsigned int uint;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8;
typedef short int int16_t;
typedef int int32;
typedef int int32_t;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64_t;
typedef unsigned long long uintptr_t;

typedef char byte;
typedef uint16 word;
typedef uint32 dword;

typedef long unsigned int size_t;

#define true 1
#define false 0

#define NULL 0

#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)
