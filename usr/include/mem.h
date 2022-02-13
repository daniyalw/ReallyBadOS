#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t malloc(int size);
extern void free(void *buf);

#ifdef __cplusplus
}
#endif
