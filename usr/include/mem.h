#pragma once

#include "_header.h"

#include <stdint.h>

C_HEADER

extern uint32_t malloc(int size);
extern void free(void *buf);

C_HEADER_END

#ifdef __cplusplus
namespace mem {

extern uint32_t alloc(int size);
extern void release(void *buf);
}
#endif
