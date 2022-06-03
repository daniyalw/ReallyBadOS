#pragma once

#include "_header.h"

C_HEADER

typedef struct {
    int sec, min, hour, day, month, year, weekday;
    bool pm;
} time_t;

extern time_t get_time();

C_HEADER_END

#ifdef __cplusplus
namespace Time {

extern time_t time();
}
#endif
