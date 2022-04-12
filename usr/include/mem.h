#pragma once

#include "_header.h"

#include <stdint.h>

C_HEADER

extern uint32_t malloc(int size);
extern void free(void *buf);

C_HEADER_END
