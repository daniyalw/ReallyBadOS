#pragma once

#define BIT_GET(num, n) (num >> n) & 1
#define BIT_SET(num, n) num |= (1 << n)
#define BIT_CLEAR(num, n) num &= (~(1 << n))
#define BIT_TOGGLE(num, n) num ^= (1 << n)
